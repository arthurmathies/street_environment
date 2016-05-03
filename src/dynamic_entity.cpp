#include "sensor_utils/dynamic_entity.h"

namespace sensor_utils {
DynamicEntity::DynamicEntity() : m_position(0, 0),lastPositon(0,0), m_viewDirection(1, 0),lastViewDirection(1,0),
    m_velocity(0),lastVelocity(0), moveDirection(1, 0),lastMoveDirection(1,0) {}

void DynamicEntity::updatePosition(const lms::math::vertex2f &position,
                                   const lms::math::vertex2f &viewDirection) {
    this->lastPositon = this->m_position;
    this->m_position = position;
    this->lastViewDirection = this->m_viewDirection;
    this->m_viewDirection = viewDirection;
}

void DynamicEntity::updateVelocity(float velocity,
                    const lms::math::vertex2f &moveDirection) {
    this->lastVelocity = this->m_velocity;
    this->m_velocity = velocity;
    this->lastMoveDirection = this->moveDirection;
    this->moveDirection = moveDirection;
}

float DynamicEntity::movedDistance() const {
    return this->m_position.distance(this->lastPositon);
}

lms::math::vertex2f DynamicEntity::deltaPosition() const{
    return this->m_position-lastPositon;
}

lms::math::vertex2f DynamicEntity::localDeltaPosition() const  {
    return (this->m_position-lastPositon).rotate(-lastPositon.angle());
}

float DynamicEntity::deltaVelocity() const {
    return this->m_velocity - this->lastVelocity;
}

float DynamicEntity::deltaX() const{
    return m_position.x -lastPositon.x;
}

float DynamicEntity::deltaY() const{
    return m_position.y -lastPositon.y;
}

float DynamicEntity::deltaPhi() const{
    return m_viewDirection.angle()-lastViewDirection.angle();

}

void DynamicEntity::updateTurnRate(float turnRate){
    this->lastTurnRate = this->m_turnRate;
    this->m_turnRate = turnRate;
}

float DynamicEntity::deltaTurnRate() const{
    return (this->m_turnRate - this->lastTurnRate);
}
}//sensor_utils
