#include <pybind11/pybind11.h>
#include "pet.hpp"
namespace py = pybind11;

PYBIND11_MODULE(frogs, m) {
    m.def("pet_name", [](const pets::Pet &pet) { return pet.name(); });
}
