#include <pybind11/pybind11.h>

namespace py = pybind11;

PYBIND11_MODULE(example, m) {
    m.doc() = "pybind11 example plugin"; // optional module docstring
    m.def("floats_only", [](double f) { return 0.5 * f; }, py::arg("f").noconvert());
    m.def("floats_preferred", [](double f) { return 0.5 * f; }, py::arg("f"));
}
