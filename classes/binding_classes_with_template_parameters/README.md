### 绑定模板类

pybind11 还可以包装具有模板参数的类。请考虑以下类：

```cpp
struct Cat {};
struct Dog {};

template <typename PetType>
struct Cage {
    Cage(PetType& pet);
    PetType& get();
};
```

C++ 模板只能在编译时实例化，因此 PyBind11 只能包装实例化的模板类。不能包装非实例化的模板：

```cpp
// BROKEN (this will not compile)
py::class_<Cage>(m, "Cage");
    .def("get", &Cage::get);
```

必须显式指定要单独包装的每个模板/类型参数的组合。

```cpp
// ok
py::class_<Cage<Cat>>(m, "CatCage")
    .def("get", &Cage<Cat>::get);

// ok
py::class_<Cage<Dog>>(m, "DogCage")
    .def("get", &Cage<Dog>::get);
```

如果类方法具有模板参数，也可以包装这些参数，但必须再次显式指定每个实例化：

```cpp
typename <typename T>
struct MyClass {
    template <typename V>
    T fn(V v);
};

py::class<MyClass<int>>(m, "MyClassT")
    .def("fn", &MyClass<int>::fn<std::string>);
```

