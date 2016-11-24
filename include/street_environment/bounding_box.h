#ifndef STREET_ENVIRONMENT_BOUNDING_BOX_H
#define STREET_ENVIRONMENT_BOUNDING_BOX_H

#include <array>
#include <vector>

#include <lms/math/vertex.h>

namespace street_environment {

class BoundingBox {
   public:
    BoundingBox(const std::vector<lms::math::vertex2f>& points);

   private:
    // clockwise starting at minimum y minimum x
    std::array<lms::math::vertex2f, 4> m_corners;
};

using BoundedObstacles = std::vector<BoundingBox>;

}  // namespace street_environment

#endif  // STREET_ENVIRONMENT_BOUNDING_BOX_H
