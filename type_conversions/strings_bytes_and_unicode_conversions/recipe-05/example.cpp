#include <iostream>
#include <Python.h>
#include <pybind11/pybind11.h>

namespace py = pybind11;
using namespace std;

PYBIND11_MODULE(example, m) {
    m.doc() = "pybind11 example plugin"; // optional module docstring
    m.def("asymmetry",
        [](std::string s) {  // Accepts str or bytes from Python
            return s;  // Looks harmless, but implicitly converts to str
        }
    );
}
