### 隐式转换

假设有A和B两个类，A可以直接转换为B。

```cpp
py::class_<A>(m, "A")
    /// ... members ...

py::class_<B>(m, "B")
    .def(py::init<A>())
    /// ... members ...

m.def("func",
    [](const B &) { /* .... */ }
);
```

如果想func函数传入A类型的参数a，Pyhton侧需要这样写func(B(a))，而C++则可以直接使用func(a)，自动将A类型转换为B类型。

这种情形下（B有一个接受A类型参数的构造函数），我们可以使用如下声明来让Python侧也支持类似的隐式转换：

```cpp
py::implicitly_convertible<A, B>();
```

> Note: A到B的隐式转换仅在通过pybind11绑定了B类型的条件下有效。
> 为了防止失控的递归调用，隐式转换时不可重入的。

