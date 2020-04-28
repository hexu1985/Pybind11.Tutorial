#!/bin/sh

rm -rf ./build
mkdir build
cd build
#    -DCMAKE_CXX_FLAGS="-DUSE_PYBIND11_LITERALS" \
cmake -Dpybind11_DIR=~/local/share/cmake/pybind11 \
    ..
cmake --build .
ctest
