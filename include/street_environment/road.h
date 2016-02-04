#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <vector>
#include "lms/inheritance.h"
#include "lms/math/vertex.h"
#include "lms/math/polyline.h"
#include "street_environment/street_environment.h"


#ifdef USE_CEREAL
#include "lms/serializable.h"
#include "cereal/cerealizable.h"
#include "cereal/cereal.hpp"
#include "cereal/types/string.hpp"
#include "cereal/types/polymorphic.hpp"
#include "cereal/types/memory.hpp"
#include "cereal/types/vector.hpp"
#include "cereal/archives/portable_binary.hpp"
#include "cereal/types/base_class.hpp"
#endif

namespace street_environment {
enum class RoadStateType{
    UNKNOWN,STRAIGHT,STRAIGHT_CURVE,CURVE
};
struct RoadState{
    RoadState():type(RoadStateType::UNKNOWN),startDistance(0),endDistance(0),probability(0),curvature(1){}
    RoadStateType type;
    /**
     * @brief startDistance tangential distance to the start of the state
     */
    float startDistance;
    /**
     * @brief startDistance tangential distance to the end of the state
     */
    float endDistance;
    float probability;
    float curvature;
};
struct RoadStates{
    std::vector<RoadState> states;
    RoadState mostProbableState() const{
        RoadState r;
        r.type = RoadStateType::UNKNOWN;
        r.probability = 0;
        for(const RoadState &v:states){
            if(v.probability > r.probability)
                r = v;
        }
        return r;
    }
};

    enum class RoadLaneType {
        LEFT, MIDDLE, RIGHT
    };

    class RoadLane : public EnvironmentObject,public lms::math::polyLine2f
    {
        RoadLaneType m_type;
    public:
        static constexpr int TYPE = 10;
        bool isSubType(size_t hashcode) const override{
            return lms::Impl<lms::math::polyLine2f>::isSubType(hashcode,this);
        }

        RoadLane(){}

        virtual bool match(const RoadLane &obj) const{
            //doesn't handle subclasses
            if(!EnvironmentObject::match(obj)){
                return false;
            }
            //TODO

            return false;

        }
        virtual ~RoadLane() {}

        int getType()const override{
           return 0;
        }

        /**
         * @brief polarDarstellung
         * polarDarstellung[0] is the y-deviance
         * polarDarstellung[1] is the start-angle in rad
         * polarDarstellung[>1] Krümmungen relativ zum vorherigen stück
         */
        std::vector<double> polarDarstellung; //TODO english name :)
        float polarPartLength;
        RoadLaneType type() const{
            return m_type;
        }

        void type(RoadLaneType type){
            m_type = type;
        }

        #ifdef USE_CEREAL
        CEREAL_SERIALIZATION()
        // member cereal serialize method
        template <class Archive>
        void serialize( Archive & archive) {
            archive (cereal::base_class<street_environment::EnvironmentObject>(this),
                        cereal::base_class<lms::math::polyLine2f>(this),
                      m_type, polarDarstellung, polarPartLength);
        }
        #endif
    };


    typedef std::shared_ptr<RoadLane> RoadLanePtr;
}  // namespace street_environment

#ifdef USE_CEREAL

namespace cereal {

template <class Archive>
struct specialize<Archive, street_environment::RoadLane, cereal::specialization::member_serialize> {};
  // cereal no longer has any ambiguity when serializing street_environment::RoadLane

}  // namespace cereal


//CEREAL_REGISTER_TYPE(street_environment::RoadLane)
//CEREAL_REGISTER_DYNAMIC_INIT(street_environment)
#endif

#endif /* ENVIRONMENT_H */
