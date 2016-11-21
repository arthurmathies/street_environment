if(USE_CONAN)
cmake_minimum_required(VERSION 2.8.12)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()
endif(USE_CONAN)

set(SOURCES
    "src/street_environment.cpp"
    "src/dynamic_entity.cpp"
    "src/obstacle.cpp"
    "src/roadmatrix.cpp"
)

set(HEADERS
    "include/street_environment/street_environment.h"
    "include/street_environment/obstacle.h"
    "include/street_environment/road.h"
    "include/street_environment/crossing.h"
    "include/street_environment/trajectory.h"
    "include/street_environment/start_line.h"
    "include/street_environment/car.h"
    "include/street_environment/dynamic_entity.h"
    "include/street_environment/pose.h"
    "include/street_environment/roadmatrix.h"
)

#set compiler flags
if((${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang") OR (${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU"))
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wextra -Wreturn-type -Wpedantic ")
endif()


include_directories("include" "codegen")
file(GLOB matlab "codegen/lib/objectTracker/*.cpp" "codegen/lib/objectTracker/*.h")
message(${matlab})
add_library(street_environment SHARED ${SOURCES} ${HEADERS} ${matlab})

if(USE_CONAN)
target_link_libraries(street_environment PRIVATE ${CONAN_LIBS})
message("Compile street_environment testing executable")

set(TESTS
    "src/test/serialization.cpp"
    "src/test/compiles.cpp"
)
add_executable(street_environment_test ${TESTS})
target_link_libraries(street_environment_test PRIVATE street_environment ${CONAN_LIBS})
add_test(StreetEnvironment street_environment_test)
else()
target_link_libraries(street_environment PRIVATE lms_math lmscore)

if(LOCAL_GMOCK)
set(TESTS
    "src/test/roadmatrix_test.cpp"
)
add_executable(street_environment_test ${TESTS})
target_link_libraries(street_environment_test PRIVATE
    gtest gmock gmock_main street_environment lms_math)
endif(LOCAL_GMOCK)

endif(USE_CONAN)
