set ( SOURCES 
    "src/street_environment.cpp"
    "src/obstacle.cpp"
)

set (HEADERS 
    "include/street_environment/street_environment.h"
    "include/street_environment/obstacle.h"
    "include/street_environment/road.h"
    "include/street_environment/crossing.h"
    "include/street_environment/trajectory.h"
    "include/street_environment/start_line.h"
    "include/street_environment/car.h"
    "include/street_environment/dynamic_entity.h"

    #Deprecated
    "include/sensor_utils/car.h"

)

include_directories("include" "codegen")
file(GLOB matlab "codegen/lib/objectTracker/*.cpp" "codegen/lib/objectTracker/*.h")
message(${matlab})
add_library (street_environment SHARED ${SOURCES} ${HEADERS} ${matlab})
target_link_libraries (street_environment PRIVATE math_lib lmscore)


set (TESTS
    "src/test/serialization.cpp"
)

if(USE_GOOGLETEST)
    message("Compile street_environment testing executable")
    add_executable(street_environment_test ${TESTS})
    target_link_libraries(street_environment_test PRIVATE street_environment math_lib gtest gtest_main lmscore)
    add_test(StreetEnvironment street_environment_test)
endif()
