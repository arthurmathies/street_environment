#ifndef OBSTACLE_H
#define OBSTACLE_H

#include "lms/math/vertex.h"
#include "street_environment.h"

#ifdef USE_CEREAL
#include "lms/serializable.h"
#include "cereal/cerealizable.h"
#include "cereal/cereal.hpp"
#include "cereal/types/polymorphic.hpp"
#include "cereal/archives/portable_binary.hpp"
#include <cereal/types/base_class.hpp>
#endif

namespace street_environment{
/**
 * @brief A dynamic entity can be the vehicle itself but also every other
 * moving obstacle.
 */
struct Obstacle:public EnvironmentObject
#ifdef USE_CEREAL
    , public lms::Serializable
#endif
{

    //TODO die Delta-Werte passen beim 1. Aufrufen nicht!
    /**
     * @brief Global position of the entity. x and y are given in meters.
     */
    lms::math::vertex2f position;

    /**
     * @brief Direction vector that points to the direction the entity is
     * looking at.
     */
    lms::math::vertex2f viewDirection;

    /**
     * @brief Velocity in m/s of the entity.
     */
    float velocity;

    /**
     * @brief Direction vector that points to the direction the entity is
     * driving to.
     */
    lms::math::vertex2f moveDirection;

    /**
     * @brief Global position of the entity in the last cycle.
     */
    lms::math::vertex2f lastPositon;

    /**
     * @brief Velocity of the entity in the last cycle.
     */
    float lastVelocity;

    Obstacle();

    /**
     * @brief Set the position for the current cycle. Should be called only
     * once per cycle.
     * @param position global position of the entity
     * @param viewDirection direction vector of the view of the entity
     */
    void updatePosition(const lms::math::vertex2f &position,
                        const lms::math::vertex2f &viewDirection);

    /**
     * @brief Set the velocity for the current cycle. Should be called only
     * once per cycle.
     * @param velocity velocity of the entity
     * @param moveDirection direction vector of the movement of the entity
     */
    void updateVelocity(float velocity,
                        const lms::math::vertex2f &moveDirection);

    /**
     * @brief Distance travelled compared to the last cycle.
     * @return distance in m
     */
    float movedDistance() const;

    /**
     * @brief Difference in velocity to the last cycle.
     * @return delta velocity in m/s
     */
    float deltaVelocity() const;

    // cereal implementation
    #ifdef USE_CEREAL
        //get default interface for datamanager
        CEREAL_SERIALIZATION()
    #endif

};

} //street_environment

#ifdef USE_CEREAL
CEREAL_REGISTER_TYPE(street_environment::Obstacle)
#endif

namespace cereal {
    template <class Archive>
    struct specialize<Archive, street_environment::Obstacle, cereal::specialization::non_member_serialize> {};
      // cereal no longer has any ambiguity when serializing street_environment::Obstacle

    template<class Archive>
    void serialize(Archive & archive, street_environment::Obstacle &obs) {
        archive (cereal::base_class<street_environment::EnvironmentObject>(&obs),
                 obs.position, obs.viewDirection, obs.velocity,
                 obs.moveDirection, obs.lastPositon, obs.lastVelocity);
    }
}  // namespace cereal

#endif /* OBSTACLE_H */
