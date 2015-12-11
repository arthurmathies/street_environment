#ifndef STREET_ENVIRONMENT_START_LINE_H
#define STREET_ENVIRONMENT_START_LINE_H
#include "obstacle.h"
namespace street_environment{
class StartLine:public Obstacle{
public:
    static constexpr int TYPE = 3;

    int getType() const override{
       return TYPE;
    }
};
} //namespace street_environment

#endif //STREET_ENVIRONMENT_START_LINE_H
