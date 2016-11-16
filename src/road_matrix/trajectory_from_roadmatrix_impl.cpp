#include "street_environment/road_matrix/trajectory_from_roadmatrix_impl.h"

#include <math.h>
#include <stdlib.h>

#include "lms/math/vertex.h"

int TrajectoryFromRoadmatrixImpl::valueFunction(
    const street_environment::RoadMatrixCell& cell,
    const street_environment::RoadMatrix& roadMatrix) {
    const int perfectTrajectory = roadMatrix.width() * (3.0 / 4);
    const int laneValueStep = 100 / perfectTrajectory;
    int value = 100 - (abs(perfectTrajectory - cell.y) * laneValueStep);
    const int obstacleClearanceCells =
        ceil(m_obstacleClearanceMeter / roadMatrix.cellLength());
    for (int x = 0; x <= obstacleClearanceCells; x++) {
        if ((cell.x + x < roadMatrix.length()) &&
            (roadMatrix.cell(cell.x + x, cell.y).hasObstacle)) {
            return value;
        }
        if ((cell.x - x >= 0) &&
            (roadMatrix.cell(cell.x - x, cell.y).hasObstacle)) {
            return value;
        }
    }
    const int carWidthCells = ceil(m_carWidthMeter / roadMatrix.cellWidth());
    value += carWidthCells * 100;
    return value;
}

std::unique_ptr<LanePieceMatrix>
TrajectoryFromRoadmatrixImpl::createLanePieceMatrix(
    const street_environment::RoadMatrix& roadMatrix) {
    const int carWidthCells = ceil(m_carWidthMeter / roadMatrix.cellWidth());

    const int numLanes = roadMatrix.width() - carWidthCells + 1;
    std::unique_ptr<LanePieceMatrix> lanePieceMatrix(new LanePieceMatrix(
        roadMatrix.length(), std::vector<LanePiece>(numLanes)));
    for (int x = 0; x < roadMatrix.length(); x++) {
        for (int y = 0; y < numLanes; y++) {
            int value = 0;
            LanePiece lanePiece;
            for (int i = 0; i < carWidthCells; i++) {
                const street_environment::RoadMatrixCell& cell =
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
    street_environment::TrajectoryPoint prevTp;
    street_environment::TrajectoryPoint curTp;
    // (0,0) is the position of the car at point 0. This is used to get the
    // direction of the first trajectory point.
    prevTp.position = lms::math::vertex2f(0, 0);

    for (const LanePiece& piece : lanePieceTrajectory) {
        curTp.position =
            (piece.cells.front().points[1] + piece.cells.back().points[2]) / 2;
        curTp.velocity = 1;
        curTp.directory = (curTp.position - prevTp.position).normalize();
        trajectory.push_back(curTp);
        prevTp = curTp;
    }

    return true;
}
