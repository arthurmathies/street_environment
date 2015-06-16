#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <vector>
#include <lms/imaging/warp.h>
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
#endif

namespace street_environment {

    enum class RoadLaneType {
        LEFT, MIDDLE, RIGHT
    };

    class RoadLane:public lms::math::polyLine2f,public EnvironmentObject
    {
        RoadLaneType m_type;
    public:
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
        //cereal methods
        template<class Archive>
        void save(Archive & archive) const {
            archive (m_type, polarDarstellung, polarPartLength);
        }

        template<class Archive>
        void load(Archive & archive) {
            archive(m_type, polarDarstellung, polarPartLength);
        }
    #endif

    };


    typedef std::shared_ptr<RoadLane> RoadLanePtr;
}

#ifdef USE_CEREAL
CEREAL_REGISTER_TYPE(street_environment::RoadLane)
#endif
#endif /* ENVIRONMENT_H */
