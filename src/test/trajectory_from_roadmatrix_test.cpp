#include "street_environment/road_matrix/trajectory_from_roadmatrix_impl.h"

#include <vector>

#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "lms/math/polyline.h"
#include "street_environment/obstacle.h"
#include "street_environment/road_matrix/road_matrix.h"
#include "street_environment/street_environment.h"

using lms::math::polyLine2f;
using lms::math::vertex2f;

namespace street_environment {

TEST(TrajectoryFromRoadmatrix, valueFunction) {
    polyLine2f straightLine;
    const int lineLength = 20;
    for (int i = 0; i < lineLength; i++) {
        straightLine.points().push_back(vertex2f(i * 0.1, 0.2));
    }

    RoadMatrix roadMatrix;
    const float laneWidth = 0.4;
    const int cellsPerLane = 4;
    const float cellLength = 0.1;
    roadMatrix.aroundLine(straightLine, laneWidth, cellsPerLane, cellLength);

    std::vector<EnvironmentObjectPtr> obstacles;

    street_environment::ObstaclePtr obst1(new street_environment::Obstacle());
    obst1->addPoint(lms::math::vertex2f(0.55, 0.05));
    obstacles.push_back(obst1);

    roadMatrix.markEnvironmentObjects(obstacles);

    TrajectoryFromRoadmatrixImpl trajectory_creator;
    trajectory_creator.setCarWidthMeter(0.2);
    trajectory_creator.setObstacleClearanceMeter(0.5);
    trajectory_creator.calculateCycleConstants(roadMatrix);

    EXPECT_EQ(
        trajectory_creator.valueFunction(roadMatrix.cell(0, 0), roadMatrix),
        12);
    EXPECT_EQ(
        trajectory_creator.valueFunction(roadMatrix.cell(0, 5), roadMatrix), 5);
}

}  // namespace street_environment
