#include <iostream>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
namespace py = pybind11;

void print_list(const std::vector<int> &my_list) {
    for (auto item : my_list)
        std::cout << item << " ";
    std::cout << std::flush;
}

PYBIND11_MODULE(example, m) {
    m.def("print_list", print_list, "A function which print_list");
}
