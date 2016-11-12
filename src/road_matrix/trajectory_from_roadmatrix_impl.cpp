#include "street_environment/road_matrix/trajectory_from_roadmatrix_impl.h"

#include <math.h>
#include <stdlib.h>

int TrajectoryFromRoadmatrixImpl::valueFunction(
    const street_environment::RoadMatrixCell& cell,
    const street_environment::RoadMatrix& roadMatrix) {
    int perfectTrajectory = roadMatrix.width() * (3.0 / 4);
    int laneValueStep = 100 / perfectTrajectory;
    int value = 100 - (abs(perfectTrajectory - cell.y) * laneValueStep);
    int obstacleClearanceCells =
        ceil(m_obstacleClearanceMeter / roadMatrix.cellLength());
    for (int x = 0; x <= obstacleClearanceCells; x++) {
        if ((cell.x + x < roadMatrix.length()) &&
            (roadMatrix.cell(cell.x + x, cell.y).hasObstacle())) {
            return value;
        }
        if ((cell.x - x >= 0) &&
            (roadMatrix.cell(cell.x - x, cell.y).hasObstacle())) {
            return value;
        }
    }
    int carWidthCells = ceil(m_carWidthMeter / roadMatrix.cellWidth());
    value += carWidthCells * 100;
    return value;
}

std::unique_ptr<LanePieceMatrix>
TrajectoryFromRoadmatrixImpl::createLanePieceMatrix(
    const street_environment::RoadMatrix& roadMatrix) {
    int carWidthCells = ceil(m_carWidthMeter / roadMatrix.cellWidth());

    int numLanes = roadMatrix.width() - carWidthCells;
    std::unique_ptr<LanePieceMatrix> lanePieceMatrix(new LanePieceMatrix(
        roadMatrix.length(), std::vector<LanePiece>(numLanes)));
    for (int x = 0; x < roadMatrix.length(); x++) {
        for (int y = 0; y < numLanes; y++) {
            int value = 0;
            LanePiece lanePiece;
            for (int i = 0; i < carWidthCells; i++) {
                street_environment::RoadMatrixCell cell =
                    roadMatrix.cell(x, y + i);
                value += valueFunction(cell, roadMatrix);
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
    std::unique_ptr<LanePieceTrajectory> cellLane(new LanePieceTrajectory);
    for (const auto& pieces : lanePieceMatrix) {
        if (pieces.size() > 0) {
            const LanePiece* a = &pieces[0];
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

bool TrajectoryFromRoadmatrixImpl::fillTrajectory(
    const LanePieceTrajectory& lanePieceTrajectory,
    street_environment::Trajectory& trajectory) {
    street_environment::TrajectoryPoint tp;
    tp.velocity = 1;
    for (const auto& piece : lanePieceTrajectory) {
        float tp_x =
            (piece.cells.front().points[1].x + piece.cells.back().points[2].x) /
            2;
        float tp_y =
            (piece.cells.front().points[1].y + piece.cells.back().points[2].y) /
            2;
        tp.position = lms::math::vertex2f(tp_x, tp_y);
        trajectory.push_back(tp);
    }
    return true;
}
