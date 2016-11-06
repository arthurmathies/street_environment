#ifndef STREET_ENVIRONMENT_ROAD_MATRIX_H_
#define STREET_ENVIRONMENT_ROAD_MATRIX_H_

#include <vector>

#include "lms/math/vertex.h"
#include "lms/math/polyline.h"

namespace street_environment {

//from http://stackoverflow.com/questions/2049582/how-to-determine-if-a-point-is-in-a-2d-triangle
struct RoadMatrixCell {
    bool hasObstacle;
    lms::math::vertex2f points[4];

    RoadMatrixCell(): hasObstacle(false) {}

    bool contains(lms::math::vertex2f p) const {
        return lms::math::pointInTriangle(p, points[0], points[1], points[2])
               || lms::math::pointInTriangle(p, points[0], points[2], points[3]);
    }
};

/**
 * @brief RoadMatrix stores a Cell representation of the road.
 */
class RoadMatrix {
    int m_width;
    int m_length;

    std::vector<lms::math::vertex2f> m_points;
    std::vector<std::vector<RoadMatrixCell>> m_cells;

    RoadMatrixCell initCell(int x, int y) const {
        RoadMatrixCell c;
        int arrayLength = m_length + 1;
        c.points[0] = m_points[x + arrayLength * y];
        c.points[1] = m_points[x + 1 + arrayLength * y];
        c.points[2] = m_points[x + 1 + arrayLength * (y + 1)];
        c.points[3] = m_points[x + arrayLength * (y + 1)];
        return c;
    }

    void initCells() {
        m_cells.clear();
        std::vector<RoadMatrixCell> tmp;
        tmp.resize(width());
        m_cells.resize(length(), tmp);
        for (int x = 0; x < m_length; x++) {
            for (int y = 0; y < m_width; y++) {
                m_cells[x][y] = initCell(x, y);
            }
        }
    }

  public:
    const RoadMatrixCell &cell(int x, int y) const {
        return m_cells[x][y];
    }
    RoadMatrixCell &cell(int x, int y) {
        return m_cells[x][y];
    }

    int width() const {
        return m_width;
    }
    int length() const {
        return m_length;
    }

    void aroundLine(const lms::math::polyLine2f &line, float laneWidth,
                    int cellsPerLane) {
        float cellWidth = laneWidth / cellsPerLane;
        float cellLength = cellWidth;
        lms::math::polyLine2f scaledLine =
            line.getWithDistanceBetweenPoints(cellLength);
        m_width = cellsPerLane * 2;
        m_length = scaledLine.points().size() - 1;

        m_points.clear();
        for (int i = -cellsPerLane; i <= cellsPerLane; i++) {
            lms::math::polyLine2f top =
                scaledLine.moveOrthogonal(cellWidth * i);
            m_points.insert(std::end(m_points), std::begin(top.points()),
                            std::end(top.points()));
        }

        initCells();
    }
};

} // namespace street_environment

#endif // STREET_ENVIRONMENT_ROAD_MATRIX_H_
