#ifndef STREET_ENVIRONMENT_ROAD_MATRIX_H_
#define STREET_ENVIRONMENT_ROAD_MATRIX_H_

#include <vector>

#include "lms/math/polyline.h"
#include "lms/math/vertex.h"

namespace street_environment {

struct RoadMatrixCell {
    // X and y coordinates of the cell in a RoadMatrix representation.
    int x, y;
    bool hasObstacle;
    lms::math::vertex2f points[4];

    RoadMatrixCell() : hasObstacle(false) {}

    bool contains(lms::math::vertex2f p) const {
        return lms::math::pointInTriangle(p, points[3], points[1], points[0]) ||
               lms::math::pointInTriangle(p, points[3], points[2], points[1]);
    }
};

/**
 * @brief RoadMatrix stores a Cell representation of the road.
 */
class RoadMatrix {
   public:
    const RoadMatrixCell &cell(int x, int y) const { return m_cells[x][y]; }
    RoadMatrixCell &cell(int x, int y) { return m_cells[x][y]; }

    int width() const { return m_width; }
    int length() const { return m_length; }

    float cellWidth() const { return m_cellWidth; }
    float cellLength() const { return m_cellLength; }

    void aroundLine(const lms::math::polyLine2f &line, float laneWidth,
                    int cellsPerLane, float cellLength);

    /**
     * @brief Tries to find a cell that contains v.
     * Returns false if no cell is found.
     * Returns true and sets 'foundCell' to a copy of the found cell if a cell
     * is found. If v is on the edge of two or more cells 'foundCell' will be the first
     * first(smallest x, smallest y) of those cells.
     */
    bool findCell(const lms::math::vertex2f &v,
                  street_environment::RoadMatrixCell *foundCell);

   private:
    int m_width;
    int m_length;

    float m_cellWidth;
    float m_cellLength;

    std::vector<lms::math::vertex2f> m_points;
    std::vector<std::vector<RoadMatrixCell>> m_cells;

    RoadMatrixCell initCell(int x, int y) const;
    void initCells();
};

}  // namespace street_environment

#endif  // STREET_ENVIRONMENT_ROAD_MATRIX_H_
