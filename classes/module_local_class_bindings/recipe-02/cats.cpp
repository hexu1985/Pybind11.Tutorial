#include <pybind11/pybind11.h>
#include "cats.hpp"
namespace py = pybind11;

PYBIND11_MODULE(cats, m) {
    // Binding for external library class:
    py::class_<pets::Pet>(m, "Pet")
        .def("get_name", &pets::Pet::name);

    // Binding for local extending class:
    py::class_<Cat, pets::Pet>(m, "Cat")
        .def(py::init<std::string>());
}
