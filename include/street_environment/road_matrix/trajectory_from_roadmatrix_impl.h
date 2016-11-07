#ifndef STREET_ENVIRONMENT_TRAJECTORY_FROM_ROADMATRIX_IMPL_H_
#define STREET_ENVIRONMENT_TRAJECTORY_FROM_ROADMATRIX_IMPL_H_

#include <vector>
#include <memory>

#include "street_environment/road_matrix/road_matrix.h"
#include "street_environment/trajectory.h"

struct LanePiece {
    std::vector<street_environment::RoadMatrixCell> cells;
    int value;
};

using LanePieceMatrix = std::vector<std::vector<LanePiece>>;
using LanePieceTrajectory = std::vector<LanePiece>;

struct TrajectoryFromRoadmatrixImpl {
    int valueFunction(const street_environment::RoadMatrixCell& cell,
                      const street_environment::RoadMatrix& roadMatrix);

    std::unique_ptr<LanePieceMatrix> createLanePieceMatrix(
        const street_environment::RoadMatrix& roadMatrix);

    std::unique_ptr<LanePieceTrajectory> getOptimalLanePieceTrajectory(
        const LanePieceMatrix& lanePieceMatrix);

    bool fillTrajectory(const LanePieceTrajectory&
                        lanePieceTrajectory,
                        street_environment::Trajectory& trajectory);

    void setCarWidthMeter(float carWidthMeter) {
        m_carWidthMeter = carWidthMeter;
    }
    void setObstacleClearanceMeter(float obstacleClearanceMeter) {
        m_obstacleClearanceMeter = obstacleClearanceMeter;
    }

  private:
    float m_carWidthMeter;
    float m_obstacleClearanceMeter;
};

#endif // STREET_ENVIRONMENT_TRAJECTORY_FROM_ROADMATRIX_IMPL_H_
