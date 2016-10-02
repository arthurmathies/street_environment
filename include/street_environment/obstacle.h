#ifndef OBSTACLE_H
#define OBSTACLE_H

#include "lms/math/vertex.h"
#include "street_environment/road.h"

#include "lms/serializable.h"
#include "cereal/cerealizable.h"
#include "cereal/cereal.hpp"
#include "cereal/types/polymorphic.hpp"
#include "cereal/archives/portable_binary.hpp"
#include "cereal/types/base_class.hpp"

namespace street_environment{
/**
 * @brief A dynamic entity can be the vehicle itself but also every other
 * moving obstacle.
 */
class Obstacle:public EnvironmentObject, public lms::Serializable
{
    //Kalman stuff
    /**
     * @brief state
     * s0 = pos tang //arcLength
     * s1 = vel tang
     * s2 = pos orth //orthLength
     * s3 = vel orth
     */
    double state[4];
    double oldState[4];
    double stateCovariance[16];

    /**
     * @brief stores the position kalman!
     */
    lms::math::vertex2f m_position;
    lms::math::vertex2f m_viewDirection;
    /**
     * @brief m_width of the obstacle orth to view_dir
     */
    float m_width;
    bool m_validKalman;

public:
    static constexpr int TYPE = 1;
    virtual int getType() const override{
       return TYPE;
    }
    virtual ~Obstacle() {}
    /**
     * @brief validKalman
     * @return true if the last m_tmpPosition was used in the kalman
     */
    bool validKalman() const;

    virtual bool match(const Obstacle &obj) const;
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

    lms::math::vertex2f position() const;
    lms::math::vertex2f viewDirection() const;
    void viewDirection(const lms::math::vertex2f &v);
    float width() const;
    void width(float w);

    /**
     * @brief kalman
     * @param middle
     * @param distanceMoved Distance that the obstacle will be translated
     */
    void kalman(const street_environment::RoadLane &middle,float distanceMoved);

    /**
     * @brief distanceTang
     * @return the length to the object tangential to the road
     */
    float distanceTang() const;
    /**
     * @brief distanceOrth
     * @return  the length to the object orthogonal to the road
     */
    float distanceOrth() const;
    /**
     * @brief velocityTang
     * @return the velocity tang to the road
     */
    float velocityTang() const;
    /**
     * @brief velocityOrth
     * @return the velocity orth to the road
     */
    float velocityOrth() const;

    template<class Archive>
    void serialize(Archive & archive) {
        archive (
            cereal::base_class<street_environment::EnvironmentObject>(this),
            m_position, m_viewDirection, m_width);
    }

    // cereal implementation
    //get default interface for datamanager
    CEREAL_SERIALIZATION()

};
typedef std::shared_ptr<Obstacle> ObstaclePtr;

} //street_environment

//CEREAL_REGISTER_TYPE(street_environment::Obstacle)

namespace cereal {
    template <class Archive>
    struct specialize<Archive, street_environment::Obstacle, cereal::specialization::member_serialize> {};
      // cereal no longer has any ambiguity when serializing street_environment::Obstacle
}  // namespace cereal

#endif /* OBSTACLE_H */
