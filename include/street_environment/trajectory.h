#ifndef STREET_ENVIRONMENT_TRAJECTORY_H
#define STREET_ENVIRONMENT_TRAJECTORY_H
#include "lms/math/polyline.h"

namespace street_environment {
    class Trajectory: public lms::math::polyLine2f{
    public:
        struct RoadChange{
            RoadChange(){}
            RoadChange(int index, bool left):changeRoadIndex(index),changeToLeft(left){}
            int changeRoadIndex;
            bool changeToLeft;
        };
        std::vector<RoadChange> changes;
        virtual void lmsSerialize(std::ostream &os) const override{
            (void)os;
            //TODO
        }
        virtual void lmsDeserialize(std::istream &is) override{
            (void)is;
            //TODO
        }

        virtual bool isSubType(size_t hashcode) const override{
            return hashcode == typeid(lms::math::polyLine2f).hash_code();
        }
        void addChange(int index, bool left){
            changes.push_back(RoadChange(index,left));
        }
    };

    struct TrajectoryPoint{
        lms::math::vertex2f position;
        lms::math::vertex2f directory;
        float velocity;
    };

}
#endif //STREET_ENVIRONMENT_TRAJECTORY_H
