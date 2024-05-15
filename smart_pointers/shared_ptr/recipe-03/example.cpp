#include <iostream>
#include <pybind11/pybind11.h>
namespace py = pybind11;

class Child : public std::enable_shared_from_this<Child> { };

class Parent {
public:
    Parent() : child(std::make_shared<Child>()) { }
    Child *get_child() { return child.get(); }
private:
    std::shared_ptr<Child> child;
};

PYBIND11_MODULE(example, m) {
    py::class_<Child, std::shared_ptr<Child>>(m, "Child");

    py::class_<Parent, std::shared_ptr<Parent>>(m, "Parent")
        .def(py::init<>())
        .def("get_child", &Parent::get_child);
}
