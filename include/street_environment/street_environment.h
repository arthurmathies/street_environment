#ifndef SENSOR_ENVIRONMENT_H
#define SENSOR_ENVIRONMENT_H
#include <vector>
#include <memory>
#include "lms/deprecated.h"

#include "lms/inheritance.h"
#ifdef USE_CEREAL
#include "lms/serializable.h"
#include "cereal/cerealizable.h"
#include "cereal/cereal.hpp"
#include "cereal/types/memory.hpp"
#include "cereal/types/vector.hpp"
#include "cereal/types/string.hpp"
#include "cereal/access.hpp"
#include "cereal/types/polymorphic.hpp"

CEREAL_FORCE_DYNAMIC_INIT(street_environment)
#endif

namespace street_environment {
class EnvironmentObject :public virtual lms::Inheritance
{
private:
    std::string m_name;
    float m_trust;
    float m_trustLast;
public:
    EnvironmentObject():m_trust(0){

    }
    /**
     * @brief setTrust
     * @param trust value between 0 and 1
     */
    void setTrust(float trust){
        if(trust < 0 || trust > 1){
            throw "invalid trust: "+std::to_string(trust);
        }
        m_trustLast = m_trust;
        m_trust = trust;
    }

    float getLastTrust(){
        return m_trustLast;
    }

    float getDeltaTrust(){
        return m_trust - m_trustLast;
    }

    /**
     * @brief trust
     * @return trust you have in the obstacle
     */
    int trust() const{
        return m_trust;
    }

    virtual ~EnvironmentObject() {}

    virtual bool isSubType(size_t hashcode) const override{
        (void)hashcode;
        return false;
    }

    template <typename T>
    DEPRECATED std::shared_ptr<T> getCopyAsPtr() const{
        return std::shared_ptr<T>(new T(*static_cast<const T*>(this)));
    }


    template <typename T>
    DEPRECATED T& getAsReference() const{
        return *static_cast<T*>(this);
    }
    virtual int getType() const = 0;

    std::string name() const{
        return m_name;
    }
    void name(const std::string &name){
        m_name = name;
    }

    template <class Archive>
    void serialize( Archive & ar ) {
        ar(m_name);
    }

    virtual bool match(const EnvironmentObject &obj) const{
        return obj.getType() == getType();
    }
};

template<typename T>
class Environment
#ifdef USE_CEREAL
    : public lms::Serializable
#endif
{
public:
    virtual ~Environment() {}

    const std::shared_ptr<EnvironmentObject> getObjectByName(std::string name) const{
        for(const std::shared_ptr<EnvironmentObject> &o: objects){
            if(o->name() == name){
                return o;
            }
        }
        return nullptr;
    }
    std::vector<std::shared_ptr<T>> objects;

    #ifdef USE_CEREAL
        //get default interface for datamanager
        CEREAL_SERIALIZATION()

        template<class Archive>
        void serialize(Archive &archive) {
            archive(objects);
        }
    #endif
};
class RoadLane;
class Obstacle;
typedef Environment<EnvironmentObject> EnvironmentObjects;
typedef Environment<RoadLane> EnvironmentRoadLane;
typedef Environment<Obstacle> EnvironmentObstacles;
}  // namespace street_environment

#ifdef USE_CEREAL
namespace cereal {

template <class Archive, typename T>
struct specialize<Archive, street_environment::Environment<T>, cereal::specialization::member_serialize> {};
  // cereal no longer has any ambiguity when serializing street_environment::Environment

}  // namespace cereal
#endif // USE_CEREAL

#endif /* SENSOR_ENVIRONMENT_H */


