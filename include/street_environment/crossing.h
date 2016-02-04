#ifndef LMS_STREET_ENVIRONMENT_CROSSING_H
#define LMS_STREET_ENVIRONMENT_CROSSING_H
#include "obstacle.h"
#include "lms/time.h"

namespace street_environment{
/**
 * @brief A dynamic entity can be the vehicle itself but also every other
 * moving obstacle.
 */
class Crossing:public Obstacle
{
private:
    int m_blockCounter;
    lms::Time m_startStop;
    float m_stopTime;

public:
    static constexpr int TYPE = 2;
    virtual int getType() const override{
       return Crossing::TYPE;
    }
    bool foundOppositeStopLine; //not that nice
    bool startStop(){
        if(m_startStop == lms::Time::ZERO){
            m_startStop = lms::Time::now();
            return true;
        }
        return false;
    }
    bool hasToStop() const{
        if(m_startStop != lms::Time::ZERO){
            return m_startStop.since().toFloat() < m_stopTime; //Time that we have to wait for an obstacle magic number
        }
        return true;
    }

    lms::Time stopTime(){
        return m_startStop;
    }

    Crossing():m_blockCounter(0),m_startStop(lms::Time::ZERO),m_stopTime(2),foundOppositeStopLine(0){
    }

    virtual bool match(const Crossing &obj) const{
        if(!Obstacle::match(obj)){
            return false;
        }
        //TODO
        return false;
    }

    void blocked(bool blocked){
        if(blocked){
            m_blockCounter++;
            if(m_blockCounter < 2) //TODO HACK
                m_blockCounter = 2;
            if(m_blockCounter > 10){
                m_blockCounter = 10;
            }
        }else{
            m_blockCounter--;
        }
    }

    bool blocked() const{
        return m_blockCounter <= 0;
    }

    // cereal implementation
    #ifdef USE_CEREAL
        //get default interface for datamanager
        CEREAL_SERIALIZATION()

        template<class Archive>
        void serialize(Archive & archive) {
            archive (
                cereal::base_class<street_environment::Obstacle>(this),
                m_blockCounter, m_startStop);
        }
    #endif

};

typedef std::shared_ptr<Crossing> CrossingPtr;
}//street_environment
#endif //LMS_STREET_ENVIRONMENT_CROSSING_H
