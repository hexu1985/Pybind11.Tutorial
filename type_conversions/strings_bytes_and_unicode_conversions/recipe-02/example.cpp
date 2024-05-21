#include <iostream>
#include <pybind11/pybind11.h>

namespace py = pybind11;
using namespace std;

PYBIND11_MODULE(example, m) {
    m.doc() = "pybind11 example plugin"; // optional module docstring
    m.def("std_string_return",
        []() {
            return std::string("This string needs to be UTF-8 encoded");
        }
    );
}
