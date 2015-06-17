#include "street_environment/obstacle.h"
extern "C"{
#include "objectTracker.h"
}
namespace street_environment{
Obstacle::Obstacle() : position(0, 0), viewDirection(1, 0),
    velocity(0), moveDirection(1, 0), lastPositon(0, 0), lastVelocity(0) {

    for(int i = 0; i < 9; i++){
        stateCovariance[i] = 0;
    }
    stateCovariance[0] = 1;
    stateCovariance[4] = 1;
    stateCovariance[8] = 1;
    init = true;
}

void Obstacle::updatePosition(const lms::math::vertex2f &position,const street_environment::RoadLane &middle) {
    //this->position = position;
    //return;

    double trustModel = 0.1;
    double trustMeasure = 0.1;
    double movedPos = 0;
    const emxArray_real_T *laneModel = emxCreate_real_T(middle.polarDarstellung.size(),1);
    for(uint i = 0; i < middle.polarDarstellung.size(); i++){
        laneModel->data[i] = middle.polarDarstellung[i];
    }
    //Kalman everthing
    emxArray_real_T *measureX = emxCreate_real_T(1,1);
    measureX->data[0]=position.x;
    emxArray_real_T *measureY = emxCreate_real_T(1,1);
    measureY->data[0]=position.y;

    objectTracker(1,laneModel, middle.polarPartLength, state, stateCovariance, trustModel, trustMeasure, measureX, measureY, movedPos);
    std::cout <<"Obstacle-input: " << position.x << " "<<position.y<<" "<<middle.polarPartLength<<std::endl;
    double arcLength = state[0];
    std::cout <<"KALMAN-vals: " <<arcLength << " " <<  state[1] << " "<< state[2]<<std::endl;

    this->lastVelocity = this->velocity;
    double currentLength = 0;

    for(int i = 1; i < (int)middle.points().size(); i++){
        double dd = middle.points()[i-1].distance(middle.points()[i]);
        if(currentLength +dd > arcLength){
            lms::math::vertex2f d = (middle.points()[i] -middle.points()[i-1]).normalize();
            //setzen der position
            updateVelocity(state[1],d);
            d*=(arcLength-currentLength);
            //setzt die aktuelle position zur alten
            this->lastPositon = this->position;
            std::cout << "obstacle-dd: "<<d.x << " " <<d.y<<std::endl;
            this->position = middle.points()[i-1]+d;
            break;
        }else{
            currentLength += dd;
        }
    }

    //TODO
    std::cout<<"PRINT stateCovariance"<<std::endl;
    for(int i = 0; i < 9; i++){
        std::cout << std::to_string(stateCovariance[i])<<" ; ";
        if(i == 2 || i == 5 || i == 8){
            std::cout <<std::endl;
        }
    }


    this->viewDirection = viewDirection;
    init = false;
}

void Obstacle::updateVelocity(float velocity,
                    const lms::math::vertex2f &moveDirection) {
    this->lastVelocity = this->velocity;
    this->velocity = velocity;
    this->moveDirection = moveDirection;
}

float Obstacle::movedDistance() const {
    return this->position.distance(this->lastPositon);
}

float Obstacle::deltaVelocity() const {
    return this->velocity - this->lastVelocity;
}
} //street_environment
