#!/bin/sh

rm -rf ./build
mkdir build
cd build
cmake -Dpybind11_DIR=~/local/share/cmake/pybind11 \
    ..
cmake --build .
ctest
