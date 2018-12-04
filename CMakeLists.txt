cmake_minimum_required(VERSION 2.8.7)
project(betax-router CXX)
#set (CURL_STATICLIB true)
set (TARGET_XML "overpass-api")

include_directories (include)
ADD_DEFINITIONS(
    -std=c++11 
    #-DCURL_STATICLIB
)

set(SOURCES_XML src/overpass-api.cpp)

add_executable(${TARGET_XML} ${SOURCES_XML})

find_path (LIBCURL_INCLUDE_DIR curl/curl.h HINTS ${LIBCURL_DIR}/include)
include_directories(${LIBCURL_INCLUDE_DIR})
#add_dependencies(${TARGET_XML} libcurl)

set (LIBS ${LIBS} boost_program_options boost_filesystem boost_system)
set (LIBS ${LIBS} curl)

target_link_libraries(${TARGET_XML} ${LIBS})

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
find_package(Osmium REQUIRED)

FIND_PACKAGE( Boost 1.58 COMPONENTS program_options REQUIRED )
INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )

set (CMAKE_CXX_STANDARD 11)

add_executable(parse-xml src/parse-xml.cpp)
set_pthread_on_target(parse-xml)
target_link_libraries(parse-xml ${OSMIUM_IO_LIBRARIES} pthread z expat bz2)
TARGET_LINK_LIBRARIES(parse-xml LINK_PUBLIC ${Boost_LIBRARIES} )

add_executable(list-bus-stop src/list-bus-stop.cpp)
set_pthread_on_target(list-bus-stop)
target_link_libraries(list-bus-stop ${OSMIUM_IO_LIBRARIES} pthread z expat bz2)
TARGET_LINK_LIBRARIES(list-bus-stop LINK_PUBLIC ${Boost_LIBRARIES} )
