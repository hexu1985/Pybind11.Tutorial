#include <pybind11/pybind11.h>
#include "pet.hpp"
namespace py = pybind11;

PYBIND11_MODULE(example, m) {
    py::class_<Pet>(m, "Pet")
        .def(py::init<const std::string &, int>())
        .def("set", static_cast<void (Pet::*)(int)>(&Pet::set), "Set the pet's age")
        .def("set", static_cast<void (Pet::*)(const std::string &)>(&Pet::set), "Set the pet's name");
}
