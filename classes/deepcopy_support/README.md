### 深拷贝支持

Python通常在赋值中使用引用。有时需要一个真正的拷贝，以防止修改所有的拷贝实例。
Python的copy模块提供了这样的拷贝能力。

在Python3中，带pickle支持的类自带深拷贝能力。
但是，自定义`__copy__`和`__deepcopy__`方法能够提高拷贝的性能。
在Python2.7中，由于pybind11只支持cPickle，要想实现深拷贝，用户必须实现这个两个方法。

对于一些简单的类，可以使用拷贝构造函数来实现深拷贝。如下所示：

```cpp
py::class_<Copyable>(m, "Copyable")
    .def("__copy__",  [](const Copyable &self) {
        return Copyable(self);
    })
    .def("__deepcopy__", [](const Copyable &self, py::dict) {
        return Copyable(self);
    }, "memo"_a);
```
