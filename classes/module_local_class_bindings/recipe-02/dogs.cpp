#include <pybind11/pybind11.h>
#include "dogs.hpp"
namespace py = pybind11;

PYBIND11_MODULE(dogs, m) {
    // Binding for external library class:
    py::class_<pets::Pet>(m, "Pet")
        .def("name", &pets::Pet::name);

    // Binding for local extension class:
    py::class_<Dog, pets::Pet>(m, "Dog")
        .def(py::init<std::string>());
}
