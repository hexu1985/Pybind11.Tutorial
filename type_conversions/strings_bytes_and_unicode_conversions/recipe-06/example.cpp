#include <iostream>
#include <Python.h>
#include <pybind11/pybind11.h>

namespace py = pybind11;
using namespace std;

PYBIND11_MODULE(example, m) {
    m.doc() = "pybind11 example plugin"; // optional module docstring
    m.def("pass_char", [](char c) { return c; });
    m.def("pass_wchar", [](wchar_t w) { return w; });
}
