#include "street_environment/obstacle.h"

#include "lib/objectTracker/objectTracker.h"
#include "lib/objectTracker/objectTracker_emxAPI.h"

namespace street_environment{
Obstacle::Obstacle() : m_position(0, 0){

    for(int i = 0; i < 16; i++){
        stateCovariance[i] = 0;
    }
    stateCovariance[0] = 1;
    stateCovariance[5] = 1;
    stateCovariance[10] = 1;
    stateCovariance[15] = 1;
    for(int i = 0; i < 4; i++){
        state[i] = 0;
        oldState[i] = 0;
    }
    m_init = true;
}

bool Obstacle::validKalman() const{
    return m_validKalman;
}

bool Obstacle::match(const Obstacle &obj) const{
    if(!EnvironmentObject::match(obj)){
        return false;
    }
    //TODO use boundingBox
    if(validKalman()){
        //check if they are both on the same line
        //if(lms::math::sgn(distanceOrth()) == lms::math::sgn(obj.distanceOrth())){
            if(fabs(distanceTang()-obj.distanceTang()) < 0.3){//TODO magic number
                return true;
          //  }
        }
    }else{
    //TODO use measurement accuracy
        return (position().distance(obj.position())<0.3);
    }
    return false;
}

lms::math::vertex2f Obstacle::position() const{
    return m_position;
}

lms::math::vertex2f Obstacle::viewDirection() const{
    return m_viewDirection;
}

void Obstacle::viewDirection(const lms::math::vertex2f &v){
    m_viewDirection = v.normalize();
}

float Obstacle::width() const{
    return m_width;
}

void Obstacle::width(float w){
    m_width = w;
}

void Obstacle::updatePosition(const lms::math::vertex2f &position) {
    this->m_position = position;
    m_validKalman = false;
}

void Obstacle::kalman(const street_environment::RoadLane &middle, float distanceMoved){
    //simple(distanceMoved);
    //TODO
}

float Obstacle::distanceTang() const{
    return state[0];
}
float Obstacle::velocityTang() const{
    return state[1];
}
float Obstacle::distanceOrth() const{
    return state[2];
}
float Obstacle::velocityOrth() const{
    return state[3];
}

} //street_environment
