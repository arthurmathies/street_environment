#ifndef STREET_ENVIRONMENT_TRAJECTORY_FROM_ROADMATRIX_IMPL_H_
#define STREET_ENVIRONMENT_TRAJECTORY_FROM_ROADMATRIX_IMPL_H_

#include <vector>
#include <memory>

#include "street_environment/road_matrix/road_matrix.h"

struct LanePiece {
    std::vector<street_environment::RoadMatrixCell> cells;
    int value;
};

using LanePieceMatrix = std::vector<std::vector<LanePiece>>;
using LanePieceTrajectory = std::vector<LanePiece>;

struct TrajectoryFromRoadmatrixImpl {
    int valueFunction(const street_environment::RoadMatrixCell& cell);

    std::unique_ptr<LanePieceMatrix> createLanePieceMatrix(
        int carWidth, const street_environment::RoadMatrix& roadMatrix);

    std::unique_ptr<LanePieceTrajectory> getOptimalLanePieceTrajectory(
        const LanePieceMatrix& lanePieceMatrix);
};

#endif // STREET_ENVIRONMENT_TRAJECTORY_FROM_ROADMATRIX_IMPL_H_
