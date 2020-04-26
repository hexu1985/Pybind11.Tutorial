git clone https://github.com/pybind/pybind11.git pybind11
cd pybind11
mkdir build
cd build
cmake -DCMAKE_INSTALL_PREFIX=~/local ..
cmake --build . --target install
