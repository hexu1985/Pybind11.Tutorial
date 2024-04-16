### 接收`*args`和`**kwatgs`参数

Python的函数可以接收任意数量的参数和关键字参数：
```python
def generic(*args, **kwargs):
    ...  # do something with args and kwargs
```

我们也可以通过pybind11来创建这样的函数：
```c++
void generic(py::args args, const py::kwargs& kwargs) {
    /// .. do something with args
    if (kwargs)
        /// .. do something with kwargs
}

/// Binding code
m.def("generic", &generic);
```

`py::args`继承自`py::tuple`，`py::kwargs`继承自`py::dict`。

