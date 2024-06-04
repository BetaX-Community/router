BetaX router
============

# Problem

Suggest one or more bus line(s) to user from geolocation

# Binaries

`overpass-api`: Request Overpass API for ways and nodes relative to each bus route

`parse-xml`: Parse the result of Overpass API to draw the path of each bus line in a map

`list-bus-stop`: List the bus lines with their associated data and
creates a text file with the list of bus stops for each bus line.

`test-json`: Transform the XML data from Overpass API into JSON format

# Usage

* Dependencies

- libosmium
- boost

* Build

```sh
cd build
cmake ..
make
```

* Run

> Assuming that you want to store the generated data in `data/` directory.

> The binaries are prefixed with `../build` here but you can add the
`build/` directory into your `PATH` as well.

```sh
cd data

../build/overpass-api -a <lon min> -b <lat min> -c <lon max> -d <lat max> -f <output file name>

../build/parse-xml <file name>

../build/test-json <file name>
```

# Project structure

```
README.md
CMakeLists.txt
cmake/
  FindBoost.cmake
  FindOsmium.cmake
src/
  overpass-api.cpp
  overpass-api.h
  parse-xml.cpp
  test-json.cpp
build/
data/
```
