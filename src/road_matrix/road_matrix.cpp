#include "street_environment/road_matrix/road_matrix.h"

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

bool RoadMatrix::markBadPosition(const lms::math::vertex2f &v, float badness) {
    for (int x = 0; x < length(); x++) {
        for (int y = 0; y < width(); y++) {
            RoadMatrixCell &rmc = cell(x, y);
            if (rmc.contains(v)) {
                rmc.badness += badness;
                if (rmc.badness < 0) {
                    rmc.badness = 0;
                } else if (rmc.badness > 1) {
                    rmc.badness = 1;
                }
                return true;
            }
        }
    }
    return false;
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

void RoadMatrix::markEnvironmentObjects(
    const std::vector<EnvironmentObjectPtr> &envObjects) {
    for (EnvironmentObjectPtr ptr : envObjects) {
        if (ptr->getType() == Obstacle::TYPE) {
            std::shared_ptr<Obstacle> obst =
                std::dynamic_pointer_cast<Obstacle>(ptr);
            for (const lms::math::vertex2f &v : obst->points()) {
                markBadPosition(v, 1);
            }
        }
    }
}

}  // namespace street_environment
