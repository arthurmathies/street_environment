#include "street_environment/bounding_box.h"

namespace street_environment {

BoundingBox::BoundingBox(const std::vector<lms::math::vertex2f>& points) {
    float minX = points.at(0).x;
    float maxX = minX;
    float minY = points.at(0).y;
    float maxY = minY;

    for (const auto& point : points) {
        if (point.x < minX) {
            minX = point.x;
        }
        if (point.x > maxX) {
            maxX = point.x;
        }
        if (point.y < minY) {
            minY = point.y;
        }
        if (point.y > maxY) {
            maxY = point.y;
        }
    }

    m_corners[0] = lms::math::vertex2f(minX, minY);
    m_corners[1] = lms::math::vertex2f(maxX, minY);
    m_corners[2] = lms::math::vertex2f(maxX, maxY);
    m_corners[3] = lms::math::vertex2f(minX, maxY);
}

}  // namespace street_environment
