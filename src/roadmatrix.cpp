#include "street_environment/roadmatrix.h"

#include <memory>
#include <vector>

#include "street_environment/obstacle.h"
#include "street_environment/street_environment.h"

namespace street_environment {

RoadMatrixCell RoadMatrix::initCell(int x, int y) const {
    RoadMatrixCell c;
    c.x = x;
    c.y = y;
    int arrayLength = m_length + 1;
    c.points[0] = m_points[x + arrayLength * y];
    c.points[1] = m_points[x + 1 + arrayLength * y];
    c.points[2] = m_points[x + 1 + arrayLength * (y + 1)];
    c.points[3] = m_points[x + arrayLength * (y + 1)];
    return c;
}

void RoadMatrix::initCells() {
    m_cells.clear();
    m_cells.resize(length(), std::vector<RoadMatrixCell>(width()));
    for (int x = 0; x < length(); x++) {
        for (int y = 0; y < width(); y++) {
            m_cells[x][y] = initCell(x, y);
        }
    }
}

/**
 * markObstacle currently has an early return if a cell that contains the
 * vertex v is found. That optimizes runtime but does not take into account
 * that a point might be on the edge of more than one cell. With the current
 * implementation this does not matter, but should be noted for future
 * improvements on this function.
 */
void RoadMatrix::markObstacle(const lms::math::vertex2f &v) {
    for (int x = 0; x < length(); x++) {
        for (int y = 0; y < width(); y++) {
            RoadMatrixCell &rmc = cell(x, y);
            if (rmc.contains(v)) {
                rmc.hasObstacle = true;
                return;
            }
        }
    }
}

void RoadMatrix::aroundLine(const lms::math::polyLine2f &line, float laneWidth,
                            int cellsPerLane, float cellLength) {
    m_cellWidth = laneWidth / cellsPerLane;
    m_cellLength = cellLength;
    lms::math::polyLine2f scaledLine =
        line.getWithDistanceBetweenPoints(m_cellLength);
    m_width = cellsPerLane * 2;
    m_length = scaledLine.points().size() - 1;

    m_points.clear();
    for (int i = -cellsPerLane; i <= cellsPerLane; i++) {
        lms::math::polyLine2f top = scaledLine.moveOrthogonal(m_cellWidth * i);
        m_points.insert(std::end(m_points), std::begin(top.points()),
                        std::end(top.points()));
    }

    initCells();
}

// TODO(arthurmathies): Make the marking of Obstacles more efficient by
// narrowing down the search space (take into account curving of the street).
void RoadMatrix::markEnvironmentObjects(
    const std::vector<EnvironmentObjectPtr> &envObjects) {
    for (EnvironmentObjectPtr ptr : envObjects) {
        if (ptr->getType() == Obstacle::TYPE) {
            std::shared_ptr<Obstacle> obst =
                std::dynamic_pointer_cast<Obstacle>(ptr);
            for (const lms::math::vertex2f &v : obst->points()) {
                markObstacle(v);
            }
        }
    }
}

}  // namespace street_environment
