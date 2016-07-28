#include <street_environment/dynamic_entity.h>

namespace street_environment {

DynamicEntity::DynamicEntity() : m_position(0, 0), m_viewDirection(1, 0),
    m_velocity(0), moveDirection(1, 0) {}

void DynamicEntity::updatePosition(const lms::math::vertex2f &position,
                                   const lms::math::vertex2f &viewDirection) {
    this->m_position = position;
    this->m_viewDirection = viewDirection;
}

void DynamicEntity::updateVelocity(float velocity,
                    const lms::math::vertex2f &moveDirection) {
    this->m_velocity = velocity;
    this->moveDirection = moveDirection;
}

float DynamicEntity::movedDistance() const {
    return 0; //TODO
}

lms::math::vertex2f DynamicEntity::deltaPosition() const{
    return lms::math::vertex2f(0,0); //TODO
}

lms::math::vertex2f DynamicEntity::localDeltaPosition() const  {
    return lms::math::vertex2f(0,0); //TODO; //TODO
}

float DynamicEntity::deltaPhi() const{
    return 0; //TODO

}

void DynamicEntity::updateTurnRate(float turnRate){
    //this->lastTurnRate = this->m_turnRate;
    this->m_turnRate = turnRate;
}

}//sensor_utils
