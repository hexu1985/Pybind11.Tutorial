### 静态属性

静态属性也可以像普通属性一样公开getter和setter方法。
隐式的self参数仍然存在，并在Python中用于传递Pythontype子类实例。
我们通常在C++侧忽略这个参数，下面的例子演示了如何使用lambda表达式做为getter函数，并忽略self参数。

```cpp
py::class_<Foo>(m, "Foo")
    .def_property_readonly_static("foo", [](py::object /* self */) { return Foo(); });
```
