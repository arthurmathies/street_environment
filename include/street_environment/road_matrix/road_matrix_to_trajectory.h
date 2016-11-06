#ifndef STREET_ENVIRONMENT_ROAD_MATRIX_TO_TRAJECTORY_H_
#define STREET_ENVIRONMENT_ROAD_MATRIX_TO_TRAJECTORY_H_

#include <vector>
#include <memory>

#include "lms/math/vertex.h"
#include "lms/math/polyline.h"
#include "street_environment/road_matrix/road_matrix.h"

namespace road_matrix_to_trajectory {

struct LanePiece {
    std::vector<street_environment::RoadMatrixCell> cells;
    int value;
};

using LanePieceMatrix = std::vector<std::vector<LanePiece>>;
using LanePieceTrajectory = std::vector<LanePiece>;

int valueFunction(const street_environment::RoadMatrixCell& cell);

std::unique_ptr<LanePieceMatrix> createLanePieceMatrix(
    int carWidth, const street_environment::RoadMatrix& roadMatrix);

std::unique_ptr<LanePieceTrajectory> getOptimalLanePieceTrajectory(
    const LanePieceMatrix& lanePieceMatrix);

} // namespace road_matrix_to_trajectory

#endif // STREET_ENVIRONMENT_ROAD_MATRIX_H_
