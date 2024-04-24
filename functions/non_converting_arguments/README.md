### Non-converting参数

有些参数可能支持类型转换，如：

- 通过`py::implicitly_convertible<A,B>()`进行隐式转换
- 将整形变量传给入参为浮点类型的函数
- 将非复数类型（如float）传给入参为`std::complex<float>`类型的函数

有时这种转换并不是我们期望的，我们可能更希望绑定代码抛出错误，而不是转换参数。通过py::arg来调用.noconvert()方法可以实现这个事情。

```cpp
m.def("floats_only", [](double f) { return 0.5 * f; }, py::arg("f").noconvert());
m.def("floats_preferred", [](double f) { return 0.5 * f; }, py::arg("f"));
```

尝试进行转换时，将抛出TypeError异常：

```python
>>> floats_preferred(4)
2.0
>>> floats_only(4)
Traceback (most recent call last):
  File "<stdin>", line 1, in <module>
TypeError: floats_only(): incompatible function arguments. The following argument types are supported:
    1. (f: float) -> float

Invoked with: 4
```

该方法可以与缩写符号`_a`和默认参数配合使用，像这样py::arg().noconvert()。


