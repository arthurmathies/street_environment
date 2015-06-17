set ( SOURCES 
	"src/street_environment.cpp"
    "src/obstacle.cpp"
)

set (HEADERS 
	"include/street_environment/street_environment.h"
    "include/street_environment/obstacle.h"
    "include/street_environment/road.h"
)

set (TESTS
    "src/test/serialization.cpp"
)

include_directories("include")
add_library (street_environment SHARED ${SOURCES} ${HEADERS})
#target_link_libraries (street_environment PRIVATE lms)

if(USE_GOOGLETEST)
    message("Compile street_environment testing executable")
    add_executable(street_environment_test ${TESTS})
    target_link_libraries(street_environment_test PRIVATE street_environment gtest gtest_main)
    add_test(StreetEnvironment street_environment_test)
endif()
