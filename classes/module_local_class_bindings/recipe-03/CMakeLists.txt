# define minimum cmake version
cmake_minimum_required(VERSION 3.10 FATAL_ERROR)

# project name and supported language
project(example LANGUAGES CXX)

add_subdirectory(pybind11)
pybind11_add_module(dogs 
    pet.hpp
    dogs.cpp)

pybind11_add_module(cats
    pet.hpp
    cats.cpp)

