#include <iostream>
#include <Python.h>
#include <pybind11/pybind11.h>

namespace py = pybind11;
using namespace std;

PYBIND11_MODULE(example, m) {
    m.doc() = "pybind11 example plugin"; // optional module docstring
    m.def("return_bytes",
        []() {
            std::string s("\xba\xd0\xba\xd0");  // Not valid UTF-8
            return py::bytes(s);  // Return the data without transcoding
        }
    );
}
