#include <pybind11/pybind11.h>
#include "pet.hpp"
#include "polymorphic_pet.hpp"
namespace py = pybind11;

PYBIND11_MODULE(example, m) {

#include "pet.def"
#include "pet_store.def"
#include "polymorphic_pet.def"
}

