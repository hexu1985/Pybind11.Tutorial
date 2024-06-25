#include <iostream>
#include <pybind11/pybind11.h>
#include <pybind11/functional.h>
namespace py = pybind11;

int func_arg(const std::function<int(int)> &f) {
    return f(10);
}

std::function<int(int)> func_ret(const std::function<int(int)> &f) {
    return [f](int i) {
        return f(i) + 1;
    };
}

py::cpp_function func_cpp() {
    return py::cpp_function([](int i) { return i+1; },
       py::arg("number"));
}

PYBIND11_MODULE(example, m) {
    m.def("func_arg", &func_arg);
    m.def("func_ret", &func_ret);
    m.def("func_cpp", &func_cpp);
}

