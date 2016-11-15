#ifndef STREET_ENVIRONMENT_ROAD_MATRIX_H_
#define STREET_ENVIRONMENT_ROAD_MATRIX_H_

#include <vector>

#include "lms/math/polyline.h"
#include "lms/math/vertex.h"
#include "street_environment/street_environment.h"

namespace street_environment {

struct RoadMatrixCell {
    // X and y coordinates of the cell in a RoadMatrix representation.
    int x, y;
    // value between 0 and 1
    float badness;
    lms::math::vertex2f points[4];

    RoadMatrixCell() : badness(0) {}

    bool hasObstacle() const { return badness > 0; }

    // See
    // http://stackoverflow.com/questions/2049582/how-to-determine-if-a-point-is-in-a-2d-triangle
    bool contains(lms::math::vertex2f p) const {
        return lms::math::pointInTriangle(p, points[3], points[1], points[0]) ||
               lms::math::pointInTriangle(p, points[3], points[2], points[1]);
    }
};

/**
 * @brief RoadMatrix stores a Cell representation of the road.
 */
class RoadMatrix {
    int m_width;
    int m_length;

    float m_cellWidth;
    float m_cellLength;

    std::vector<lms::math::vertex2f> m_points;
    std::vector<std::vector<RoadMatrixCell>> m_cells;

    RoadMatrixCell initCell(int x, int y) const;
    void initCells();

    void markBadPosition(const lms::math::vertex2f &v, float badness);

   public:
    const RoadMatrixCell &cell(int x, int y) const { return m_cells[x][y]; }
    RoadMatrixCell &cell(int x, int y) { return m_cells[x][y]; }

    int width() const { return m_width; }
    int length() const { return m_length; }

    float cellWidth() const { return m_cellWidth; }
    float cellLength() const { return m_cellLength; }

    void aroundLine(const lms::math::polyLine2f &line, float laneWidth,
                    int cellsPerLane, float cellLength);

    void markEnvironmentObjects(
        const std::vector<EnvironmentObjectPtr> &envObjects);
};

}  // namespace street_environment

#endif  // STREET_ENVIRONMENT_ROAD_MATRIX_H_
