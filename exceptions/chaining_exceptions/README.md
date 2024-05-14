### 异常链（raise from）

在Python 3.3中，引入了指示异常是由其他异常引发的机制：

```python
try:
    print(1 / 0)
except Exception as exc:
    raise RuntimeError("could not divide by zero") from exc
```

pybind11 2.8版本，你可以使用py::raise_from函数来完成相同的事。它设置当前Python错误指示器，所以要继续传播异常，你应该throw py::error_already_set()（Python 3 only）。

```cpp
try {
    py::eval("print(1 / 0"));
} catch (py::error_already_set &e) {
    py::raise_from(e, PyExc_RuntimeError, "could not divide by zero");
    throw py::error_already_set();
}
```
