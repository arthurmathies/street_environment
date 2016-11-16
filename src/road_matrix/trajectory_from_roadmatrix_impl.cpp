#include "street_environment/road_matrix/trajectory_from_roadmatrix_impl.h"

#include <math.h>
#include <stdlib.h>

#include "lms/math/vertex.h"

namespace {
const int laneValueStep = 1;
}

void TrajectoryFromRoadmatrixImpl::calculateCycleConstants(
    const street_environment::RoadMatrix& roadMatrix) {
    m_carWidthCells = ceil(m_carWidthMeter / roadMatrix.cellWidth());
    m_perfectTrajectory = roadMatrix.width() * (3.0 / 4.0);
    m_numLanes = roadMatrix.width() - m_carWidthCells + 1;
    m_obstacleClearanceCells =
        ceil(m_obstacleClearanceMeter / roadMatrix.cellLength());
    m_maxCellValue = m_perfectTrajectory * laneValueStep;
}

std::unique_ptr<LanePieceMatrix>
TrajectoryFromRoadmatrixImpl::createLanePieceMatrix(
    const street_environment::RoadMatrix& roadMatrix) const {
    std::unique_ptr<LanePieceMatrix> lanePieceMatrix(new LanePieceMatrix(
        roadMatrix.length(), std::vector<LanePiece>(m_numLanes)));
    for (int x = 0; x < roadMatrix.length(); x++) {
        for (int y = 0; y < m_numLanes; y++) {
            int value = 0;
            LanePiece lanePiece;
            for (int i = 0; i < m_carWidthCells; i++) {
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
    const LanePieceMatrix& lanePieceMatrix) const {
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
    street_environment::Trajectory& trajectory) const {
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

int TrajectoryFromRoadmatrixImpl::valueFunction(
    const street_environment::RoadMatrixCell& cell,
    const street_environment::RoadMatrix& roadMatrix) const {
    int value =
        m_maxCellValue - (abs(m_perfectTrajectory - cell.y) * laneValueStep);

    for (int x = 0; x <= m_obstacleClearanceCells; x++) {
        if ((cell.x + x < roadMatrix.length()) &&
            (roadMatrix.cell(cell.x + x, cell.y).hasObstacle)) {
            return value;
        }
        if ((cell.x - x >= 0) &&
            (roadMatrix.cell(cell.x - x, cell.y).hasObstacle)) {
            return value;
        }
    }

    value += m_carWidthCells * m_maxCellValue;
    return value;
}
