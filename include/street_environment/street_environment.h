#ifndef SENSOR_ENVIRONMENT_H
#define SENSOR_ENVIRONMENT_H
#include <vector>
#include <memory>

#ifdef USE_CEREAL
#include "lms/serializable.h"
#include "cereal/cerealizable.h"
#include "cereal/cereal.hpp"
#include "cereal/types/vector.hpp"
#include "cereal/types/string.hpp"
#include "cereal/types/memory.hpp"
#include <cereal/access.hpp>
#endif

namespace street_environment {

class EnvironmentObject
{
#ifdef USE_CEREAL
    friend class cereal::access;
#endif
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
    void name(std::string name){
        m_name = name;
    }
};

class Environment
#ifdef USE_CEREAL
    : public lms::Serializable
#endif
{
public:
    const std::shared_ptr<EnvironmentObject> getObjectByName(std::string name) const{
        for(const std::shared_ptr<EnvironmentObject> &o: objects){
            if(o->name() == name){
                return o;
            }
        }
        return nullptr;
    }
    std::vector<std::shared_ptr<EnvironmentObject>> objects;

    #ifdef USE_CEREAL
        //get default interface for datamanager
        CEREAL_SERIALIZATION()

        //cereal methods
        template<class Archive>
        void save(Archive & archive) const {
            archive (objects);
        }

        template<class Archive>
        void load(Archive & archive) {
            archive(objects);
        }
    #endif
};

}  // namespace street_environment

#ifdef USE_CEREAL
namespace cereal {

template<class Archive>
void save(Archive &archive, const street_environment::EnvironmentObject &obj) {
    archive(obj.name());
}

template<class Archive>
void load(Archive &archive, street_environment::EnvironmentObject &obj) {
    std::string name;
    archive(name);
    obj.name(name);
}

}  // namespace cereal
#endif // USE_CEREAL

#endif /* SENSOR_ENVIRONMENT_H */


