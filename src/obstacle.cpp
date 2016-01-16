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

    static_assert(sizeof(state)/sizeof(double) == 4,"Obstacle::kalman: Size doesn't match idiot!");

    //Set old state
    for(uint i = 0; i < sizeof(state)/sizeof(double); i++){
        oldState[i] = state[i];
    }

    bool useKalman = (m_position.x > 0) && (distanceTang() >= 0) || m_init;
    //Kalman doesn't like values < 0
    if(useKalman){
        //get new values for kalman
        double trustModel = 0.001;
        double trustMeasure = 0.001;
        const emxArray_real_T *laneModel = emxCreate_real_T(middle.polarDarstellung.size(),1);
        for(uint i = 0; i < middle.polarDarstellung.size(); i++){
            laneModel->data[i] = middle.polarDarstellung[i];
        }

        float measureX =m_position.x;
        float measureY =m_position.y;
        //kalman it
        //HACK As the kalman doesn't work atm
        //1 for init as we abuse the kalman to calculate distanceOrth() and distanceTang()
        objectTracker(1,laneModel, middle.polarPartLength, state, stateCovariance, trustModel, trustMeasure,trustMeasure, measureX, measureY, distanceMoved,true);
    }

    // we move it by hand
    state[0] = state[0]-distanceMoved;

    //if the tang distance is smaller than 0 we move it manually backwards.  Becuase of the geometry of the road it should be fine
    //TODO I am not sure what the kalman does is m_position.x is smaller then 0
    if(!useKalman){
        this->m_position.x -= distanceMoved;
    }else{
    //Calculate the new x-y
        double currentLength = 0;
        for(int i = 1; i < (int)middle.points().size(); i++){
            double dd = middle.points()[i-1].distance(middle.points()[i]);
            if(currentLength +dd > distanceTang()){
                lms::math::vertex2f d = (middle.points()[i] -middle.points()[i-1]).normalize();
                //setzen der position
                //Bis jetzt nur tangential
                lms::math::vertex2f tang = d*(distanceTang()-currentLength);
                lms::math::vertex2f orth = d.rotateAntiClockwise90deg()*distanceOrth();
                this->m_position = middle.points()[i-1]+tang+orth;

                //Addiere den orthogonalen anteil
                break;
            }else{
                currentLength += dd;
            }
        }
    }
    m_validKalman = true;
    m_init = false;

    if(position().x != position().x){
        std::cout<<"WELL DONE"<<std::endl;
        std::cout<<distanceMoved<<std::endl;
        exit(0);
    }
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
