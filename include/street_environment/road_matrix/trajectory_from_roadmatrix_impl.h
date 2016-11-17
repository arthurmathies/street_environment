#ifndef STREET_ENVIRONMENT_TRAJECTORY_FROM_ROADMATRIX_IMPL_H_
#define STREET_ENVIRONMENT_TRAJECTORY_FROM_ROADMATRIX_IMPL_H_

#include <memory>
#include <vector>

#include "street_environment/road_matrix/road_matrix.h"
#include "street_environment/trajectory.h"

struct LanePiece {
    std::vector<street_environment::RoadMatrixCell> cells;
    int value;
    bool stop;
};

using LanePieceMatrix = std::vector<std::vector<LanePiece>>;
using LanePieceTrajectory = std::vector<LanePiece>;

struct TrajectoryFromRoadmatrixImpl {
    void calculateCycleConstants(const street_environment::RoadMatrix& roadMatrix);

    std::unique_ptr<LanePieceMatrix> createLanePieceMatrix(
        const street_environment::RoadMatrix& roadMatrix) const;

    std::unique_ptr<LanePieceTrajectory> getOptimalLanePieceTrajectory(
        const LanePieceMatrix& lanePieceMatrix) const;

    bool fillTrajectory(const LanePieceTrajectory& lanePieceTrajectory,
                        street_environment::Trajectory& trajectory) const;

    int valueFunction(const street_environment::RoadMatrixCell& cell,
                      const street_environment::RoadMatrix& roadMatrix) const;

    void setCarWidthMeter(float carWidthMeter) {
        m_carWidthMeter = carWidthMeter;
    }
    void setObstacleClearanceMeter(float obstacleClearanceMeter) {
        m_obstacleClearanceMeter = obstacleClearanceMeter;
    }

   private:
    //config values
    float m_carWidthMeter;
    float m_obstacleClearanceMeter;
    // cycle constants
    int m_cellsPerLane;
    int m_carWidthCells;
    int m_perfectTrajectory;
    int m_numLanes;
    int m_obstacleClearanceCells;
    // Defines the maximum value a cell can have without the obstacle bonus.
    int m_maxCellValue;
    // Defines the maximum value a LanePiece can have without the obstacle
    // bonus.
    int m_maxLanePieceValue;
};

#endif  // STREET_ENVIRONMENT_TRAJECTORY_FROM_ROADMATRIX_IMPL_H_
