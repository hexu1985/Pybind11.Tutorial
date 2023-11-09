### 枚举和内部类型

现在有一个含有枚举和内部类型的类：

```cpp
struct Pet {
    enum Kind {
        Dog = 0,
        Cat
    };

    struct Attributes {
        float age = 0;
    };

    Pet(const std::string &name, Kind type) : name(name), type(type) { }

    std::string name;
    Kind type;
    Attributes attr;
};
```

绑定代码如下所示：

```cpp
py::class_<Pet> pet(m, "Pet");

pet.def(py::init<const std::string &, Pet::Kind>())
    .def_readwrite("name", &Pet::name)
    .def_readwrite("type", &Pet::type)
    .def_readwrite("attr", &Pet::attr);

py::enum_<Pet::Kind>(pet, "Kind")
    .value("Dog", Pet::Kind::Dog)
    .value("Cat", Pet::Kind::Cat)
    .export_values();

py::class_<Pet::Attributes> attributes(pet, "Attributes")
    .def(py::init<>())
    .def_readwrite("age", &Pet::Attributes::age);
```

为确保嵌套类型Kind和Attributes在Pet的作用域中创建，我们必须向enum_和class_的构造函数提供Pet class_实例。
enum_::export_values()用来导出枚举项到父作用域，C++11的强枚举类型需要跳过这点。

```cpp
>>> p = Pet("Lucy", Pet.Cat)
>>> p.type
Kind.Cat
>>> int(p.type)
1L
```

枚举类型的枚举项会被导出到类__members__属性中：

```cpp
>>> Pet.Kind.__members__
{'Dog': Kind.Dog, 'Cat': Kind.Cat}
```

name属性可以返回枚举值的名称的unicode字符串，str(enum)也可以做到，但两者的实现目标不同。下面的例子展示了两者的差异：

```cpp
>>> p = Pet("Lucy", Pet.Cat)
>>> pet_type = p.type
>>> pet_type
Pet.Cat
>>> str(pet_type)
'Pet.Cat'
>>> pet_type.name
'Cat'
```

