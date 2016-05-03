#ifndef STREET_ENVIRONMENT_CAR_H
#define STREET_ENVIRONMENT_CAR_H
#include "street_environment//dynamic_entity.h"
#include "lms/time.h"

#ifdef USE_CEREAL
#include "cereal/types/vector.hpp"
#include "cereal/types/string.hpp"
#include "cereal/types/base_class.hpp"
#include "cereal/cerealizable.h"
#endif

namespace street_environment {
/**
  *TODO Add hasState()
 * @brief The Car class
 */
class Car:public DynamicEntity{
public:
    Car(){
    }
    enum class StateType{
        NOT_DEFINED,IDLE,DRIVING,PARKING,RACE
    };
    struct State
#ifdef USE_CEREAL
        : public lms::Serializable
#endif
    {
        State():priority(0),state(StateType::NOT_DEFINED),indicatorLeft(false),indicatorRight(false),startState(lms::Time::ZERO),
            endState(lms::Time::ZERO),steering_front(0),steering_rear(0),targetSpeed(0){}

        /**
         * @brief priority of the state, for example StateType::DRIVING could have priority 1, PARKING could have 2 -> the car would par
         */
        int priority;
        std::string name;
        StateType state;
        bool indicatorLeft;
        bool indicatorRight;

        /**
         * @brief startState
         */
        lms::Time startState;
        /**
         * @brief endState time till the car should do something (for example idle)
         */
        lms::Time endState;
        float steering_front, steering_rear;
        float targetSpeed;
        /**
         * @brief intime
         * @param currentTime
         * @return
         */
        bool intime(const lms::Time &currentTime) const{
            if(endState == lms::Time::ZERO && startState == lms::Time::ZERO){
                return true;
            }
            return startState < currentTime && currentTime < endState;
        }

#ifdef USE_CEREAL
    CEREAL_SERIALIZATION()

        template <class Archive>
        void serialize( Archive & archive) {
            archive(priority, name, state, startState, endState, steering_front,
                    steering_rear, targetSpeed);
        }
#endif
    };
private:
    std::vector<State> states; //TODO not sure if it should be public
    float m_localDx;
    float m_localDy;
public:

    State getPrioState() const{
        if(states.size() == 0){
            //TODO throw error
            return State();
        }
        State prio = states[0];
        for(uint i = 1; i < states.size();i++){
            if(states[i].priority > prio.priority)
                prio = states[i];
        }
        return prio;
    }

    float steeringFront() const{
        return getPrioState().steering_front;
    }

    float steeringRear() const{
        return getPrioState().steering_rear;
    }

    float targetSpeed() const{
        return getPrioState().targetSpeed;
    }

    void addState(const State &s){
        states.push_back(s);
    }

    void putState(const State &s){
        for(uint i = 0; i < states.size(); i++){
            if(states[i].name == s.name){
                states[i] = s;
                return;
            }
        }
        addState(s);
    }

    bool removeState(const std::string &name){
        for(uint i = 0; i < states.size();){
            if(states[i].name == name){
                states.erase(states.begin()+i);
                return true;
            }else{
                i++;
            }
        }
        return false;
    }

    State* getState(const std::string &name){
        for(State &s:states){
            if(s.name == name)
                return &s;
        }
        return nullptr;
    }

    /**
     * @brief validateStates removed invalid/outdated states
     * @param currentTime
     */
    void validateStates(const lms::Time &currentTime){
        for(uint i = 0; i < states.size();){
            if(!states[i].intime(currentTime)){
                states.erase(states.begin()+i);
            }else{
                i++;
            }
        }
    }

    // cereal implementation
#ifdef USE_CEREAL
    //get default interface for datamanager
    CEREAL_SERIALIZATION()

    template <class Archive>
    void serialize( Archive & archive) {
        //TODO
        archive(states,m_localDx,m_localDy);
        archive(cereal::base_class<DynamicEntity>(this));
    }
#endif
};
}//street_environment

#endif //STREET_ENVIRONMENT_CAR_H
