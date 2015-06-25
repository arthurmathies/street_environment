#ifndef SENSOR_ENVIRONMENT_H
#define SENSOR_ENVIRONMENT_H
#include <vector>
#include <memory>

#ifdef USE_CEREAL
#include "lms/serializable.h"
#include "cereal/cerealizable.h"
#include "cereal/cereal.hpp"
#include "cereal/types/memory.hpp"
#include "cereal/types/vector.hpp"
#include "cereal/types/string.hpp"
#include "cereal/access.hpp"
#include "cereal/types/polymorphic.hpp"
#endif

CEREAL_FORCE_DYNAMIC_INIT(street_environment)

namespace street_environment {
class EnvironmentObject
{
private:
    std::string m_name;
public:
    virtual ~EnvironmentObject() {}

    template <typename T>
    std::shared_ptr<T> getCopyAsPtr() const{
        return std::shared_ptr<T>(new T(*static_cast<const T*>(this)));
    }

    template <typename T>
    T& getAsReference() const{
        return *static_cast<T*>(this);
    }

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


