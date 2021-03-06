PROJECT(osm2pgrouting)
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

set (CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})
set (SHARE_DIR "/usr/share/osm2pgrouting")

FIND_PACKAGE(PostgreSQL REQUIRED)
FIND_PACKAGE(EXPAT REQUIRED)

FIND_PACKAGE(Boost)
if(Boost_INCLUDE_DIRS)
    message(STATUS "Boost headers were found here: ${Boost_INCLUDE_DIRS}")
else(Boost_INCLUDE_DIRS)
    message(FATAL_ERROR " Please check your Boost installation ") 
endif(Boost_INCLUDE_DIRS)

FIND_PACKAGE(Boost COMPONENTS program_options REQUIRED)

FILE(GLOB SRC "${CMAKE_SOURCE_DIR}/src/*.cpp" "${CMAKE_SOURCE_DIR}/src/*.h"
    "${CMAKE_SOURCE_DIR}/src/CH/*.cpp" "${CMAKE_SOURCE_DIR}/src/CH/*.h"
    "${CMAKE_SOURCE_DIR}/src/CH/contraction_hierarchies/*.cpp" "${CMAKE_SOURCE_DIR}/src/CH/contraction_hierarchies/*.hpp"
    "${CMAKE_SOURCE_DIR}/src/CH/shortest_path_algorithms/*.cpp" "${CMAKE_SOURCE_DIR}/src/CH/shortest_path_algorithms/*.hpp")

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_FILE_OFFSET_BITS=64 -std=c++0x")
set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -Wconversion -pedantic -Wextra  -frounding-math -Wno-deprecated")



message(STATUS "POSTGRESQL_INCLUDE_DIR: ${POSTGRESQL_INCLUDE_DIR}")
message(STATUS "EXPAT_INCLUDE_DIRS: ${EXPAT_INCLUDE_DIRS}")
message(STATUS "Boost_INCLUDE_DIRS: ${Boost_INCLUDE_DIRS}")
message(STATUS "POSTGRESQL_LIBRARIES: ${POSTGRESQL_LIBRARIES}")
message(STATUS "Boost_LIBRARIES: ${boost_LIBRARIES}")

INCLUDE_DIRECTORIES(src ${POSTGRESQL_INCLUDE_DIR} ${EXPAT_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS})

ADD_EXECUTABLE(osm2pgrouting ${SRC})

TARGET_LINK_LIBRARIES(osm2pgrouting ${POSTGRESQL_LIBRARIES} ${EXPAT_LIBRARIES} ${Boost_LIBRARIES} )

INSTALL(TARGETS osm2pgrouting
    RUNTIME DESTINATION "/usr/bin"
    )

INSTALL(FILES
    "${CMAKE_SOURCE_DIR}/COPYING"
    "${CMAKE_SOURCE_DIR}/Readme.md"
    "${CMAKE_SOURCE_DIR}/mapconfig.xml"
    "${CMAKE_SOURCE_DIR}/mapconfig_for_cars.xml"
    "${CMAKE_SOURCE_DIR}/mapconfig_for_bicycles.xml"
    DESTINATION "${SHARE_DIR}")


#INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )
#TARGET_LINK_LIBRARIES( osm2pgrouting ${Boost_LIBRARIES} )
