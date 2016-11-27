#ifndef STREET_ENVIRONMENT_BOUNDING_BOX_H
#define STREET_ENVIRONMENT_BOUNDING_BOX_H

#include <array>

#include <lms/math/point_cloud.h>
#include <lms/math/vertex.h>

namespace street_environment {

class BoundingBox {
   public:
    // Do not use default constructor. Only used for data channel magic.
    BoundingBox() : m_corners() {}
    BoundingBox(const lms::math::PointCloud2f& pointCloud);

    const std::array<lms::math::vertex2f, 4>& corners() const {
        return m_corners;
    }

   private:
    // clockwise starting at minimum y minimum x
    std::array<lms::math::vertex2f, 4> m_corners;
};

using BoundingBoxVector = std::vector<BoundingBox>;

}  // namespace street_environment

#endif  // STREET_ENVIRONMENT_BOUNDING_BOX_H
