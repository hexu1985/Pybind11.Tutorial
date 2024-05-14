#include <pybind11/pybind11.h>
#include "pet.hpp"
namespace py = pybind11;

PYBIND11_MODULE(module2, m) {
    m.def("create_pet", [](std::string name) { return new Pet(name); });
}
