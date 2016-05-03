#ifndef STREET_ENVIRONMENT_DYNAMIC_ENTITY_H
#define STREET_ENVIRONMENT_DYNAMIC_ENTITY_H

#include "lms/math/vertex.h"

namespace street_environment {
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
    lms::math::vertex2f lastPositon;

    /**
     * @brief Direction vector that points to the direction the entity is
     * looking at.
     */
    lms::math::vertex2f m_viewDirection;
    lms::math::vertex2f lastViewDirection;

    /**
     * @brief Velocity in m/s of the entity.
     */
    float m_velocity;
    /**
     * @brief Velocity of the entity in the last cycle.
     */
    float lastVelocity;

    /**
     * @brief Turn rate (rad/s) of the entity
     */
    float m_turnRate;
    /**
     * @brief Turn rate of the entity in the last cycle.
     */
    float lastTurnRate;

    /**
     * @brief Direction vector that points to the direction the entity is
     * driving to.
     */
    lms::math::vertex2f moveDirection;
    lms::math::vertex2f lastMoveDirection;

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
            archive(m_position,lastPositon,m_velocity,lastVelocity,m_viewDirection,lastViewDirection);
        }
    #endif
};
}//sensor_utils

#endif /* STREET_ENVIRONMENT_DYNAMIC_ENTITY_H */
