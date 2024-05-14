### 在C++中处理Python异常

当C++调用Python函数时（回调函数或者操作Python对象），若Python有异常抛出，
pybind11会将Python异常转化为pybind11::error_already_set类型的异常，它包含了一个C++字符串描述和实际的Python异常。
error_already_set用于将Python异常传回Python（或者在C++侧处理）。

| Exception raised in Python | Thrown as C++ exception type |
| -------------------------- | ---------------------------- |
| Any Python Exception       | pybind11::error_already_set  |

举个例子：

```cpp
try {
    // open("missing.txt", "r")
    auto file = py::module_::import("io").attr("open")("missing.txt", "r");
    auto text = file.attr("read")();
    file.attr("close")();
} catch (py::error_already_set &e) {
    if (e.matches(PyExc_FileNotFoundError)) {
        py::print("missing.txt not found");
    } else if (e.matches(PyExc_PermissionError)) {
        py::print("missing.txt found but not accessible");
    } else {
        throw;
    }
}
```

该方法并不适用与C++到Python的翻译，Python侧抛出的异常总是被翻译为error_already_set.


```cpp
try {
    py::eval("raise ValueError('The Ring')");
} catch (py::value_error &boromir) {
    // Boromir never gets the ring
    assert(false);
} catch (py::error_already_set &frodo) {
    // Frodo gets the ring
    py::print("I will take the ring");
}

try {
    // py::value_error is a request for pybind11 to raise a Python exception
    throw py::value_error("The ball");
} catch (py::error_already_set &cat) {
    // cat won't catch the ball since
    // py::value_error is not a Python exception
    assert(false);
} catch (py::value_error &dog) {
    // dog will catch the ball
    py::print("Run Spot run");
    throw;  // Throw it again (pybind11 will raise ValueError)
}
```
