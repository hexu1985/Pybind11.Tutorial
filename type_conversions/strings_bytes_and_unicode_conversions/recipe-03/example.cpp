#include <iostream>
#include <Python.h>
#include <pybind11/pybind11.h>

namespace py = pybind11;
using namespace std;

PYBIND11_MODULE(example, m) {
    m.doc() = "pybind11 example plugin"; // optional module docstring
    // This uses the Python C API to convert Latin-1 to Unicode
    m.def("str_output",
        []() {
            std::string s = "Send your r\xe9sum\xe9 to Alice in HR"; // Latin-1
            py::handle py_s = PyUnicode_DecodeLatin1(s.data(), s.length(), nullptr);
            if (!py_s) {
                throw py::error_already_set();
            }
            return py::reinterpret_steal<py::str>(py_s);
        }
    );
}
