#ifndef STREET_ENVIRONMENT_DYNAMIC_ENTITY_H
#define STREET_ENVIRONMENT_DYNAMIC_ENTITY_H

#include "lms/math/vertex.h"
#include <eigen3/Eigen/Eigen>
#include <vector>
namespace street_environment {

struct Pose2D{
    Eigen::Vector3f m_position;
    /**
     * @brief update
     * @param dx
     * @param dy
     * @param dphi drehung des Koordinatensystems
     */
    void update(float dx, float dy, float dphi){
        Eigen::Matrix<float,3,3> transRot;
        transRot(0,0)=cos(dphi);
        transRot(0,1)=-sin(dphi);
        transRot(0,2)=dx;
        transRot(1,0)=sin(dphi);
        transRot(1,1)=cos(dphi);
        transRot(1,2)=dy;
        transRot(2,0)=0;
        transRot(2,1)=0;
        transRot(2,2)=1;
        m_position=transRot*m_position;
    }
};
struct PoseEntity2D{

    int posesMaxSize;
    Pose2D currentPose;
    std::vector<std::pair<int,Pose2D>> poses;
    /**
     * @brief update
     * @param dx
     * @param dy
     * @param dphi drehung des Koordinatensystems
     * @param time (can be abused as id) in ms
     */
    void update(float dx, float dy, float dphi, int time){
        currentPose.update(dx,dy,dphi);
        poses.push_back({time,currentPose});
        if(posesMaxSize != -1 && (int)poses.size() > posesMaxSize){
            //TODO don't use vectorposes.
        }
    }
};

/**
 * @brief A dynamic entity can be the vehicle itself but also every other
 * moving obstacle.
 */
class DynamicEntity
        #ifdef USE_CEREAL
            : public lms::Serializable
        #endif
{
protected:
    /**
     * @brief Global position of the entity. x and y are given in meters.
     */
    lms::math::vertex2f m_position;

    /**
     * @brief Direction vector that points to the direction the entity is
     * looking at.
     */
    lms::math::vertex2f m_viewDirection;

    /**
     * @brief Velocity in m/s of the entity.
     */
    float m_velocity;

    /**
     * @brief Turn rate (rad/s) of the entity
     */
    float m_turnRate;

    /**
     * @brief Direction vector that points to the direction the entity is
     * driving to.
     */
    lms::math::vertex2f moveDirection;

public:
    DynamicEntity();
    virtual ~DynamicEntity(){}

    lms::math::vertex2f viewDirection() const{
        return m_viewDirection;
    }

    lms::math::vertex2f position()const{
        return m_position;
    }

    float velocity()const{
        return m_velocity;
    }

    float turnRate()const{
        return m_turnRate;
    }

    /**
     * @brief Set the position for the current cycle. Should be called only
     * once per cycle.
     * @param position global position of the entity
     * @param viewDirection direction vector of the view of the entity
     */
    void updatePosition(const lms::math::vertex2f &m_position,
                        const lms::math::vertex2f &m_viewDirection);

    //void updatePosition(float dx, float dy, float dphi);

    /**
     * @brief Set the velocity for the current cycle. Should be called only
     * once per cycle.
     * @param velocity velocity of the entity
     * @param moveDirection direction vector of the movement of the entity
     */
    void updateVelocity(float velocity,
                        const lms::math::vertex2f &moveDirection);


    /**
    * @brief Set the turn rate for the current cycle. Should be called only
    * once per cycle.
    * @param turnRate The turn rate of the entity
    */
    void updateTurnRate(float turnRate);

    //####Delta-Values

    lms::math::vertex2f deltaPosition() const;
    lms::math::vertex2f localDeltaPosition() const;

    /**
     * @brief Difference in velocity to the last cycle.
     * @return delta velocity in m/s
     */
    float deltaVelocity() const;
    float deltaX() const;
    float deltaY() const;
    float deltaPhi() const;
    float deltaTurnRate() const;
    float movedDistance() const;


    // cereal implementation
    #ifdef USE_CEREAL
        //get default interface for datamanager
        CEREAL_SERIALIZATION()

        template <class Archive>
        void serialize( Archive & archive) {
            archive(m_position,m_velocity,m_viewDirection);
        }
    #endif
};
}//sensor_utils

#endif /* STREET_ENVIRONMENT_DYNAMIC_ENTITY_H */
