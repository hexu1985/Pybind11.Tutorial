#include <pybind11/pybind11.h>

namespace py = pybind11;

struct Dog {
};

struct Cat {
};

PYBIND11_MODULE(animals, m) {
    py::class_<Dog>(m, "Dog").def(py::init<>());
    py::class_<Cat>(m, "Cat").def(py::init<>());
    m.def("bark", [](Dog *dog) -> std::string {
        if (dog) return "woof!"; /* Called with a Dog instance */
        else return "(no dog)"; /* Called with None, dog == nullptr */
    }, py::arg("dog").none(true));
    m.def("meow", [](Cat *cat) -> std::string {
        // Can't be called with None argument
        return "meow";
    }, py::arg("cat").none(false));
}

