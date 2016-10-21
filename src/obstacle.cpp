#include "street_environment/obstacle.h"

#include "lib/objectTracker/objectTracker.h"
#include "lib/objectTracker/objectTracker_emxAPI.h"

namespace street_environment{
Obstacle::Obstacle() : m_position(0, 0){
    m_init = true;
}


bool Obstacle::match(const Obstacle &obj) const{
    if(!EnvironmentObject::match(obj)){
        return false;
    }

    return (position().distance(obj.position())<0.3);
}

lms::math::vertex2f Obstacle::position() const{
    return m_position;
}


float Obstacle::distanceTang() const{
    return 0;
}
float Obstacle::velocityTang() const{
    return 0;
}
float Obstacle::distanceOrth() const{
    return 0;
}
float Obstacle::velocityOrth() const{
    return 0;
}

} //street_environment
