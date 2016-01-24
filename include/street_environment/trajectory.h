#ifndef STREET_ENVIRONMENT_TRAJECTORY_H
#define STREET_ENVIRONMENT_TRAJECTORY_H
#include "lms/math/polyline.h"

#ifdef USE_CEREAL
#include <cereal/types/base_class.hpp>
#include <cereal/types/vector.hpp>
#endif

namespace street_environment {
    class Trajectory: public lms::math::polyLine2f{
    public:
        struct RoadChange{
            RoadChange(){}
            RoadChange(int index, bool left):changeRoadIndex(index),changeToLeft(left){}
            int changeRoadIndex;
            bool changeToLeft;

            template<class Archive>
            void serialize(Archive &archive) {
                archive(changeRoadIndex, changeToLeft);
            }
        };
        std::vector<RoadChange> changes;
        lms::math::polyLine2f viewDirs;

        template<class Archive>
        void serialize(Archive &archive) {
            archive(cereal::base_class<lms::math::polyLine2f>(this), changes);
        }

        CEREAL_SERIALIZATION()

        virtual bool isSubType(size_t hashcode) const override{
            return hashcode == typeid(lms::math::polyLine2f).hash_code();
        }
        void addChange(int index, bool left){
            changes.push_back(RoadChange(index,left));
        }
    };

    struct TrajectoryPoint:public lms::Serializable{
        lms::math::vertex2f position;
        lms::math::vertex2f directory;
        float velocity;
        TrajectoryPoint():position(0,0),directory(1,0),velocity(0){

        }
#ifdef USE_CEREAL
        CEREAL_SERIALIZATION()

        template<class Archive>
        void serialize(Archive &ar) {
            ar(position, directory, velocity);
        }
#endif
    };

}
#endif //STREET_ENVIRONMENT_TRAJECTORY_H
