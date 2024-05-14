#include <pybind11/pybind11.h>
#include "pet.hpp"
namespace py = pybind11;

PYBIND11_MODULE(module1, m) {
    py::class_<Pet>(m, "Pet")
        .def(py::init<std::string>())
        .def_readonly("name", &Pet::name);
}
