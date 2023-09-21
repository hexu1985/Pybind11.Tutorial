# define minimum cmake version
cmake_minimum_required(VERSION 3.10 FATAL_ERROR)

# project name and supported language
project(hello LANGUAGES CXX)

#set(PythonLibsNew_FIND_VERSION 3.8)
set(PYTHON_TARGET_VER 3.8)
find_package(PythonInterp ${PYTHON_TARGET_VER} EXACT REQUIRED)

add_subdirectory(pybind11)
pybind11_add_module(hello hello.cpp)

