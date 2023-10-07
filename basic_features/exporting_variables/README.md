### 导出变量

我们可以使用`attr`函数来注册需要导出到Python模块中的C++变量。内建类型和常规对象会在指定attriutes时自动转换，也可以使用`py::cast`来显式转换。

```c++
PYBIND11_MODULE(example, m) {
    m.attr("the_answer") = 42;
    py::object world = py::cast("World");
    m.attr("what") = world;
}
```

Python中使用如下：
```
$ pyhton
>>> import example
>>> example.the_answer
42
>>> example.what
'World'
```

