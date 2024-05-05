BetaX router
============

# Problem

Suggest one or more bus line(s) to user from geolocation

# Executables

overpass-api : Request Overpass API for ways and nodes relative to each bus route
parse-xml : Parse the result of Overpass API to draw the path of each bus line in a map

# Usage

* Dependencies

- libosmium
- boost

* Build

mkdir build; cd build; cmake ..; make

* Run

./overpass-api -b <lat min> -a <lon min> -d <lat max> -c <lon max> -f <file name>

./parse-xml <file name>

# Project structure

README.md
CMakeLists.txt
cmake/
  FindBoost.cmake
  FindOsmium.cmake
src/
  overpass-api.cpp
  overpass-api.h
  parse-xml.cpp
build/
data/
