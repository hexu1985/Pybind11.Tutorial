#include <pybind11/pybind11.h>
#include "pet.hpp"

namespace py = pybind11;

PYBIND11_MODULE(example, m) {
    py::class_<Pet> pet(m, "Pet");
    pet.def(py::init<const std::string &>())
       .def_readwrite("name", &Pet::name);
    
    // Method 2: pass parent class_ object:
    py::class_<Dog>(m, "Dog", pet /* <- specify Python parent type */)
        .def(py::init<const std::string &>())
        .def("bark", &Dog::bark);
}

