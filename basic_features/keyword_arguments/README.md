### 关键字参数

使用`py::arg`可以指定函数的参数名，Python侧调用函数时可以使用关键字参数，以增加代码的可读性。
```cpp
m.def("add", &add, "A function which adds two numbers",
      py::arg("i"), py::arg("j"));
```

更简洁的写法：
```cpp
// regular notation
m.def("add1", &add, py::arg("i"), py::arg("j"));
// shorthand
using namespace pybind11::literals;
m.def("add2", &add, "i"_a, "j"_a);
```

Python使用示例：
```python
import example
example.add(i=1, j=2)  #3L
```
