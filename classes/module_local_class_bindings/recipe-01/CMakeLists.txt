# define minimum cmake version
cmake_minimum_required(VERSION 3.10 FATAL_ERROR)

# project name and supported language
project(example LANGUAGES CXX)

add_subdirectory(pybind11)
pybind11_add_module(module1 
    pet.hpp
    module1.cpp)

pybind11_add_module(module2
    pet.hpp
    module2.cpp)

