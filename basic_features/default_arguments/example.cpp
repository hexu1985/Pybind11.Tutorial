#include <iostream>
#include <pybind11/pybind11.h>
namespace py = pybind11;

int add(int i, int j) {
    std::cout << "add(int, int) called: i=" << i << ", j=" << j << std::endl;
    return i + j;
}

PYBIND11_MODULE(example, m) {
    m.doc() = "pybind11 example plugin"; // optional module docstring
    m.def("add", &add, "A function which adds two numbers",
        py::arg("i")=1, py::arg("j")=2);
}
