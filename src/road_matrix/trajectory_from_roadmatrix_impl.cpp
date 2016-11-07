#include "street_environment/road_matrix/trajectory_from_roadmatrix_impl.h"

#include <stdlib.h>

int TrajectoryFromRoadmatrixImpl::valueFunction(
    const street_environment::RoadMatrixCell& cell) {
    int value = 0;
    return value;
}

std::unique_ptr<LanePieceMatrix>
TrajectoryFromRoadmatrixImpl::createLanePieceMatrix(
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

std::unique_ptr<LanePieceTrajectory>
TrajectoryFromRoadmatrixImpl::getOptimalLanePieceTrajectory(
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
