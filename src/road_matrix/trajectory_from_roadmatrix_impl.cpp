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

bool TrajectoryFromRoadmatrixImpl::fillTrajectory(const LanePieceTrajectory&
        lanePieceTrajectory, street_environment::Trajectory& trajectory) {
    street_environment::TrajectoryPoint tp;
    tp.velocity = 1;
    for (const auto& piece : lanePieceTrajectory) {
        float tp_x = (piece.cells.front().points[1].x +
                      piece.cells.back().points[2].x) / 2;
        float tp_y = (piece.cells.front().points[1].y +
                      piece.cells.back().points[2].y) / 2;
        tp.position = lms::math::vertex2f(tp_x, tp_y);
        trajectory.push_back(tp);
    }
    return true;
}
