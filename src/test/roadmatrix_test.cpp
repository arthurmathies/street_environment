#include "street_environment/roadmatrix.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "lms/math/polyline.h"

using lms::math::polyLine2f;
using lms::math::vertex2f;

namespace street_environment {

TEST(RoadMatrix, aroundLine) {
    polyLine2f straightLine;
    int lineLength = 20;
    for (int i = 0; i < lineLength; i++) {
        straightLine.points().push_back(vertex2f(i * 0.1, 0.1));
    }

    RoadMatrix roadMatrix;
    float laneWidth = 0.4;
    int cellsPerLane = 4;
    float cellLength = 0.1;
    roadMatrix.aroundLine(straightLine, laneWidth, cellsPerLane, cellLength);
}

}  // namespace street_environment
