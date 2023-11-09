### 重载方法

重载方法即拥有相同的函数名，但入参不一样的函数：
```cpp
struct Pet {
    Pet(const std::string &name, int age) : name(name), age(age) { }

    void set(int age_) { age = age_; }
    void set(const std::string &name_) { name = name_; }

    std::string name;
    int age;
};
```

我们在绑定`Pet::set`时会报错，因为编译器并不知道用户想选择哪个重载方法。我们需要添加具体的函数指针来消除歧义。绑定多个函数到同一个Python名称，将会自动创建函数重载链。Python将会依次匹配，找到最合适的重载函数。

```cpp
py::class_<Pet>(m, "Pet")
   .def(py::init<const std::string &, int>())
   .def("set", static_cast<void (Pet::*)(int)>(&Pet::set), "Set the pet's age")
   .def("set", static_cast<void (Pet::*)(const std::string &)>(&Pet::set), "Set the pet's name");
```

在函数的文档描述中，我们可以看见重载的函数签名：
```python
>>> help(example.Pet)

class Pet(__builtin__.object)
 |  Methods defined here:
 |
 |  __init__(...)
 |      Signature : (Pet, str, int) -> NoneType
 |
 |  set(...)
 |      1. Signature : (Pet, int) -> NoneType
 |
 |      Set the pet's age
 |
 |      2. Signature : (Pet, str) -> NoneType
 |
 |      Set the pet's name
```

如果你的编译器支持C++14，也可以使用下面的语法来转换重载函数：
```cpp
py::class_<Pet>(m, "Pet")
    .def("set", py::overload_cast<int>(&Pet::set), "Set the pet's age")
    .def("set", py::overload_cast<const std::string &>(&Pet::set), "Set the pet's name");
```

这里，`py::overload_cast`仅需指定函数类型，不用给出返回值类型，以避免原语法带来的不必要的干扰(`void (Pet::*)`)。如果是基于const的重载，需要使用`py::const`标识。

```cpp
struct Widget {
    int foo(int x, float y);
    int foo(int x, float y) const;
};

py::class_<Widget>(m, "Widget")
   .def("foo_mutable", py::overload_cast<int, float>(&Widget::foo))
   .def("foo_const",   py::overload_cast<int, float>(&Widget::foo, py::const_));
```

如果你想在仅支持c++11的编译器上使用`py::overload_cast`语法，可以使用`py::detail::overload_cast_impl`来代替：

```cpp
template <typename... Args>
using overload_cast_ = pybind11::detail::overload_cast_impl<Args...>;

py::class_<Pet>(m, "Pet")
    .def("set", overload_cast_<int>()(&Pet::set), "Set the pet's age")
    .def("set", overload_cast_<const std::string &>()(&Pet::set), "Set the pet's name");
```

> Note: 如果想定义多个重载的构造函数，使用`.def(py::init<...>())`语法依次定义就好，指定关键字和默认参数的机制也还是生效的。


