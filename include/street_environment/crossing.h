#ifndef LMS_STREET_ENVIRONMENT_CROSSING_H
#define LMS_STREET_ENVIRONMENT_CROSSING_H
#include "obstacle.h"

namespace street_environment{
/**
 * @brief A dynamic entity can be the vehicle itself but also every other
 * moving obstacle.
 */
class Crossing:public Obstacle
{
private:
    bool m_blocked;

public:
    static constexpr int TYPE = 2;

    virtual bool match(const Crossing &obj) const{
        if(!Obstacle::match(obj)){
            return false;
        }
        //TODO
        return false;
    }


    int getType() const override{
       return TYPE;
    }


    void blocked(bool blocked){
        m_blocked = blocked;
    }

    bool blocked(){
        return m_blocked;
    }

    // cereal implementation
    #ifdef USE_CEREAL
        //get default interface for datamanager
        CEREAL_SERIALIZATION()

        template<class Archive>
        void serialize(Archive & archive) {
            archive (
                cereal::base_class<street_environment::Obstacle>(this),
                m_blocked);
        }
    #endif

};
}//street_environment

//TODO CEREAL
#ifdef USE_CEREAL
//CEREAL_REGISTER_TYPE(street_environment::Obstacle)
#endif

namespace cereal {
    template <class Archive>
    struct specialize<Archive, street_environment::Crossing, cereal::specialization::member_serialize> {};
      // cereal no longer has any ambiguity when serializing street_environment::Obstacle
}  // namespace cereal

#endif //LMS_STREET_ENVIRONMENT_CROSSING_H
