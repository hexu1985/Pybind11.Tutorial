#include <iostream>
#include <pybind11/pybind11.h>

int add(int i, int j) {
    std::cout << "add(int, int) called" << std::endl;
    return i + j;
}

double add(double i, double j) {
    std::cout << "add(double, double) called" << std::endl;
    return i + j;
}

PYBIND11_MODULE(example, m) {
    m.doc() = "pybind11 example plugin"; // optional module docstring
    m.def("add", static_cast<int(*)(int, int)>(&add), "A function which adds two int numbers");
    m.def("add", static_cast<double(*)(double, double)>(&add), "A function which adds two double numbers");
}
