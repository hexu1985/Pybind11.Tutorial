### 非公有析构函数

如果一个类拥有私有或保护的析构函数（例如单例类），通过pybind11绑定类时编译器将会报错。
本质的问题是std::unique_ptr智能指针负责管理实例的生命周期需要引用析构函数，即便没有资源需要回收。
Pybind11提供了辅助类py::nodelete来禁止对析构函数的调用。这种情况下，C++侧负责析构对象避免内存泄漏就十分重要。

```cpp
/* ... definition ... */

class MyClass {
private:
    ~MyClass() { }
};

/* ... binding code ... */

py::class_<MyClass, std::unique_ptr<MyClass, py::nodelete>>(m, "MyClass")
    .def(py::init<>())
```
