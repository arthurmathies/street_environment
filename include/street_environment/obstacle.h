#ifndef OBSTACLE_H
#define OBSTACLE_H

#include "lms/math/vertex.h"
#include "street_environment/road.h"

extern "C"{
#include "../object_tracker/objectTracker_emxAPI.h"
}

#ifdef USE_CEREAL
#include "lms/serializable.h"
#include "cereal/cerealizable.h"
#include "cereal/cereal.hpp"
#include "cereal/types/polymorphic.hpp"
#include "cereal/archives/portable_binary.hpp"
#include "cereal/types/base_class.hpp"
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
    virtual ~Obstacle() {}

    //Kalman stuff
    double state[4];
    double oldState[4];
    double stateCovariance[16];

    //TODO die Delta-Werte passen beim 1. Aufrufen nicht!
    /**
     * @brief stores the position before the kalman!
     */
    lms::math::vertex2f m_tmpPosition;
    bool m_validKalman;
    /**
     * @brief validKalman
     * @return true if the last m_tmpPosition was used in the kalman
     */
    bool validKalman() const;

    Obstacle();
    /**
     * @brief init used to init the kalman!
     */
    bool m_init;


    /**
     * @brief updatePosition
     * @param position
     */
    void updatePosition(const lms::math::vertex2f &position);

    lms::math::vertex2f position();

    void kalman(const street_environment::RoadLane &middle,float distanceMoved);

    /**
     * @brief getStreetDistanceTangential
     * @return the length to the object tangential
     */
    float getStreetDistanceTangential();
    /**
     * @brief getStreetDistanceOrthogonal
     * @return  the length to the object orthogonal
     */
    float getStreetDistanceOrthogonal();

    // cereal implementation
    #ifdef USE_CEREAL
        //get default interface for datamanager
        CEREAL_SERIALIZATION()

        template<class Archive>
        void serialize(Archive & archive) {
            archive (
                cereal::base_class<street_environment::EnvironmentObject>(this),
                m_tmpPosition);
        }
    #endif

};

} //street_environment

#ifdef USE_CEREAL
//CEREAL_REGISTER_TYPE(street_environment::Obstacle)
#endif

namespace cereal {
    template <class Archive>
    struct specialize<Archive, street_environment::Obstacle, cereal::specialization::member_serialize> {};
      // cereal no longer has any ambiguity when serializing street_environment::Obstacle
}  // namespace cereal

#endif /* OBSTACLE_H */
