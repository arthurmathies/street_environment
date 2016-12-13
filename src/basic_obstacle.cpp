#include "street_environment/basic_obstacle.h"

namespace street_environment {

void BasicObstacle::translate(const lms::math::vertex2f& deltaPosition) {
    for (auto& point : m_points) {
        point = point - deltaPosition;
    }
}

void BasicObstacle::rotate(float deltaAngle) {
    for (auto& point : m_points) {
        point = point.rotate(deltaAngle);
    }
}

street_environment::BoundingBox2f BasicObstacle::boundingBox() const {
    return street_environment::BoundingBox2f(m_points);
}

}  // namespace street_environment
