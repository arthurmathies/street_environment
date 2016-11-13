#include "street_environment/road_matrix/road_matrix.h"

#include <vector>

#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "lms/math/polyline.h"

using lms::math::polyLine2f;
using lms::math::vertex2f;

namespace street_environment {
TEST(RoadMatrix, aroundLine) {
    polyLine2f straightLine;
    int lineLength = 20;
    vertex2f pointOnLine;
    for (int i = 0; i < lineLength; i++) {
        pointOnLine.x = i * 0.1;
        pointOnLine.y = 0;
        straightLine.points().push_back(pointOnLine);
    }

    RoadMatrix roadMatrix;
    float laneWidth = 0.4;
    int cellsPerLane = 4;
    roadMatrix.aroundLine(straightLine, laneWidth, cellsPerLane);
}
}
