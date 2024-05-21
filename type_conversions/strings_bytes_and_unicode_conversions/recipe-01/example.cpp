#include <iostream>
#include <pybind11/pybind11.h>

namespace py = pybind11;
using namespace std;

PYBIND11_MODULE(example, m) {
    m.doc() = "pybind11 example plugin"; // optional module docstring
    m.def("utf8_test",
        [](const std::string &s) {
            cout << "utf-8 is icing on the cake.\n";
            cout << s;
            cout << flush;
        }
    );
    m.def("utf8_charptr",
        [](const char *s) {
            cout << "My favorite food is\n";
            cout << s;
            cout << flush;
        }
    );
}
