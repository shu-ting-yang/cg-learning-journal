#!/bin/bash
rm -rf build out CMakePresets.json
mkdir build
cd build
cmake .. -G "Unix Makefiles"
make

# to run it, simply execute ./triangle_glfw under /build