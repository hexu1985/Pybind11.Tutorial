### 参数默认值

pybind11不能自动地提取默认参数，因为它不属于函数类型信息的一部分。我们需要借助`arg`在绑定时指定参数默认值：
```c++
m.def("add", &add, "A function which adds two numbers",
      py::arg("i") = 1, py::arg("j") = 2);
```

更简短的声明方式：
```c++
// regular notation
m.def("add1", &add, py::arg("i") = 1, py::arg("j") = 2);
// shorthand
m.def("add2", &add, "i"_a=1, "j"_a=2);
```
