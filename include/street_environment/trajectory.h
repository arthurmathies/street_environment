#ifndef STREET_ENVIRONMENT_TRAJECTORY_H
#define STREET_ENVIRONMENT_TRAJECTORY_H
#include "lms/math/polyline.h"

#ifdef USE_CEREAL
#include <cereal/types/base_class.hpp>
#include <cereal/types/vector.hpp>
#endif

namespace street_environment {

    struct TrajectoryPoint:public lms::Serializable{
        lms::math::vertex2f position;
        lms::math::vertex2f directory;
        float velocity;
        float distanceToMiddleLane;
        /**
         * @brief isRight
         * @return true if the TrajectoryPoint is on the left side of the middle of the road
         */
        bool isRight(){
            return distanceToMiddleLane > 0;
        }

        TrajectoryPoint():position(0,0),directory(1,0),velocity(0),distanceToMiddleLane(0){

        }
        TrajectoryPoint(lms::math::vertex2f pos,lms::math::vertex2f viewDir, float velocity_, float distanceToMiddleLane_):position(pos),directory(viewDir),velocity(velocity_),distanceToMiddleLane(distanceToMiddleLane_){

        }
    #ifdef USE_CEREAL
        CEREAL_SERIALIZATION()

        template<class Archive>
        void serialize(Archive &ar) {
            ar(position, directory, velocity,distanceToMiddleLane);
        }
    #endif
    };

    class Trajectory: public std::vector<TrajectoryPoint>, public lms::Inheritance/* TODO , public lms::Serializable*/{
    public:
        template<class Archive>
        void serialize(Archive &archive) {
            archive(std::vector<TrajectoryPoint>(this));
        }
/* TODO
#ifdef USE_CEREAL
        CEREAL_SERIALIZATION()
#endif
*/
        virtual bool isSubType(size_t hashcode) const override{
            return hashcode == typeid(std::vector<TrajectoryPoint>).hash_code();
        }
    };

}
#endif //STREET_ENVIRONMENT_TRAJECTORY_H
