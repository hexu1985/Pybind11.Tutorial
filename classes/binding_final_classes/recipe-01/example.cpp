#include <pybind11/pybind11.h>
namespace py = pybind11;

struct IsFinal final {};

PYBIND11_MODULE(example, m) {
    py::class_<IsFinal>(m, "IsFinal", py::is_final());
}
