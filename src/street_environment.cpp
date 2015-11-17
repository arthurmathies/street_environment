#include "street_environment/obstacle.h"
#include "street_environment/road.h"


#ifdef USE_CEREAL
#include "cereal/archives/portable_binary.hpp"
#include "cereal/types/polymorphic.hpp"

CEREAL_REGISTER_TYPE(street_environment::RoadLane)
CEREAL_REGISTER_TYPE(street_environment::Obstacle)
CEREAL_REGISTER_DYNAMIC_INIT(street_environment)
#endif
