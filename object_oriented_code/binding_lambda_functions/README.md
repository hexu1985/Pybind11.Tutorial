### 绑定匿名函数

使用`print(p)`打印对象信息时，上面的例子会得到一些基本无用的信息。
```python
>>> print(p)
<example.Pet object at 0x10cd98060>
```

我们可以绑定一个工具函数到`__repr__`方法，来返回可读性好的摘要信息。在不改变Pet类的基础上，使用一个匿名函数来完成这个功能是一个不错的选择。

```cpp
py::class_<Pet>(m, "Pet")
    .def(py::init<const std::string &>())
    .def("setName", &Pet::setName)
    .def("getName", &Pet::getName)
    .def("__repr__",
        [](const Pet &a) {
            return "<example.Pet named '" + a.name + "'>";
        });
```

通过上面的修改，Python中的输出如下：
```python
>>> print(p)
<example.Pet named 'Molly'>
```

pybind11支持无状态和有状态的lambda闭包，即lambda表达式的`[]`是否带捕获参数。

