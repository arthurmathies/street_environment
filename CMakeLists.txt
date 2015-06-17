set ( SOURCES 
	"src/street_environment.cpp"
    "src/obstacle.cpp"
)

set (HEADERS 
	"include/street_environment/street_environment.h"
    "include/street_environment/obstacle.h"
    "include/street_environment/road.h"
)

set(OBJ_TRACKER "object_tracker")
include_directories("include")
include_directories("${OBJ_TRACKER}")
file(GLOB tracker "${OBJ_TRACKER}/*.c" "${OBJ_TRACKER}/*.h")
add_library (street_environment SHARED ${SOURCES} ${HEADERS} ${tracker})
#target_link_libraries (street_environment PRIVATE lms)
