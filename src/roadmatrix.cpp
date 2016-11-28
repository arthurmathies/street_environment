#include "street_environment/roadmatrix.h"

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

bool RoadMatrix::findCell(const lms::math::vertex2f& v,
                          street_environment::RoadMatrixCell* foundCell) {
    for (int x = 0; x < length(); x++) {
        for (int y = 0; y < width(); y++) {
            const street_environment::RoadMatrixCell& rmc = cell(x, y);
            if (rmc.contains(v)) {
                *foundCell = rmc;
                return true;
            }
        }
    }
    return false;
}

}  // namespace street_environment
