#include "street_environment/obstacle.h"
extern "C"{
#include "objectTracker.h"
}
namespace street_environment{
Obstacle::Obstacle() : m_tmpPosition(0, 0){

    for(int i = 0; i < 16; i++){
        stateCovariance[i] = 0;
    }
    stateCovariance[0] = 1;
    stateCovariance[5] = 1;
    stateCovariance[10] = 1;
    stateCovariance[15] = 1;
    m_init = true;
}

bool Obstacle::validKalman() const{
    return m_validKalman;
}

lms::math::vertex2f Obstacle::position() const{
    return m_tmpPosition;
}

void Obstacle::updatePosition(const lms::math::vertex2f &position) {
    this->m_tmpPosition = position;
    m_validKalman = false;
    //return;
}

void Obstacle::kalman(const street_environment::RoadLane &middle, float distanceMoved){
    static_assert(sizeof(state)/sizeof(double) == 4,"Obstacle::kalman: Size doesn't match idiot!");

    //Set old state
    for(uint i = 0; i < sizeof(state)/sizeof(double); i++){
        oldState[i] = state[i];
    }

    //get new values for kalman
    double trustModel = 0.1;
    double trustMeasure = 0.1;
    const emxArray_real_T *laneModel = emxCreate_real_T(middle.polarDarstellung.size(),1);
    for(uint i = 0; i < middle.polarDarstellung.size(); i++){
        laneModel->data[i] = middle.polarDarstellung[i];
    }
    emxArray_real_T *measureX = emxCreate_real_T(1,1);
    measureX->data[0]=m_tmpPosition.x;
    emxArray_real_T *measureY = emxCreate_real_T(1,1);
    measureY->data[0]=m_tmpPosition.y;
    //kalman it
    objectTracker(1,laneModel, middle.polarPartLength, state, stateCovariance, trustModel, trustMeasure,trustMeasure, measureX, measureY, distanceMoved);

    //destroy stuff
    //TODO

    //Convert the kalman-result
    std::cout <<"Obstacle-input: " << m_tmpPosition.x << " "<<m_tmpPosition.y<<" "<<middle.polarPartLength<<std::endl;
    double arcLength = state[0];
    double orthLength = state[1];
    std::cout <<"KALMAN-vals: " <<arcLength << " " <<  state[1] << " "<< state[2]<<std::endl;
    double currentLength = 0;

    for(int i = 1; i < (int)middle.points().size(); i++){
        double dd = middle.points()[i-1].distance(middle.points()[i]);
        if(currentLength +dd > arcLength){
            lms::math::vertex2f d = (middle.points()[i] -middle.points()[i-1]).normalize();
            //setzen der position
            //Bis jetzt nur tangential
            lms::math::vertex2f tang = d*(arcLength-currentLength);
            lms::math::vertex2f orth = d.rotateClockwise90deg()*orthLength;
            this->m_tmpPosition = middle.points()[i-1]+tang+orth;
            //Addiere den orthogonalen anteil
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
    m_validKalman = false;
    m_init = false;
}

float Obstacle::getStreetDistanceOrthogonal() const{
    //TODO
    return 0;
}
float Obstacle::getStreetDistanceTangential() const{
    //TODO
    return 0;
}

} //street_environment
