### Keyword-only参数

Python3提供了keyword-only参数（在函数定义中使用*作为匿名参数）：

```py
def f(a, *, b):  # a can be positional or via keyword; b must be via keyword
    pass

f(a=1, b=2)  # good
f(b=2, a=1)  # good
f(1, b=2)  # good
f(1, 2)  # TypeError: f() takes 1 positional argument but 2 were given
```

pybind11提供了`py::kw_only`对象来实现相同的功能：

```cpp
m.def("f", [](int a, int b) { /* ... */ },
      py::arg("a"), py::kw_only(), py::arg("b"));
```

注意，该特性不能与py::args一起使用。
