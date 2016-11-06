#include "street_environment/road_matrix/road_matrix_to_trajectory.h"

namespace road_matrix_to_trajectory {

int valueFunction(const street_environment::RoadMatrixCell& cell) {
    if (cell.hasObstacle) {
        return 0;
    }
    return 1;
}

std::unique_ptr<LanePieceMatrix> createLanePieceMatrix(
    int carWidth, const street_environment::RoadMatrix& roadMatrix) {
    std::unique_ptr<LanePieceMatrix> lanePieceMatrix(new LanePieceMatrix());
    for (int x = 0; x < roadMatrix.length(); x++) {
        for (int y = 0; y < roadMatrix.width() - carWidth; y++) {
            int value = 0;
            LanePiece lanePiece;
            for (int i = 0; i < carWidth; i++) {
                street_environment::RoadMatrixCell cell =
                    roadMatrix.cell(x, y + i);
                value += valueFunction(cell);
                lanePiece.cells.push_back(cell);
            }
            lanePiece.value = value;
            lanePieceMatrix->at(x).push_back(lanePiece);
        }
    }
    return lanePieceMatrix;
}

std::unique_ptr<LanePieceTrajectory> getOptimalLanePieceTrajectory(
    const LanePieceMatrix& lanePieceMatrix) {
    std::unique_ptr<LanePieceTrajectory> cellLane(new LanePieceTrajectory());
    for (const auto& pieces : lanePieceMatrix) {
        if (pieces.size() > 0) {
            const LanePiece * a = &pieces[0];
            for (const auto& piece : pieces) {
                if (piece.value > a->value) {
                    a = &piece;
                }
            }
            cellLane->push_back(*a);
        }
    }
    return cellLane;
}

} // namespace road_matrix_to_trajectory
