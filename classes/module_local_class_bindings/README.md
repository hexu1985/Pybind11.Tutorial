### 绑定Module-local类

pybind11默认将类绑定到模块的全局作用域中。这意味着模块中定义的类型，可能获得其他模块中相同类型名的结果。
示例如下：

```cpp
// In the module1.cpp binding code for module1:
py::class_<Pet>(m, "Pet")
    .def(py::init<std::string>())
    .def_readonly("name", &Pet::name);

// In the module2.cpp binding code for module2:
m.def("create_pet", [](std::string name) { return new Pet(name); });
```

```python
>>> from module1 import Pet
>>> from module2 import create_pet
>>> pet1 = Pet("Kitty")
>>> pet2 = create_pet("Doggy")
>>> pet2.name()
'Doggy'
```

有时，我们希望将一个复杂的库分割到几个Python模块中。

在某些例子中，这也会引起冲突。例如，有两个不相干的模块使用了同一个C++外部库，而且他们各自提供了这个库的自定义绑定。
当Python程序同时（直接或间接地）导入两个库时，由于外部类型的定义冲突而导致错误。

```cpp
// dogs.cpp

// Binding for external library class:
py::class_<pets::Pet>(m, "Pet")
    .def("name", &pets::Pet::name);

// Binding for local extension class:
py::class_<Dog, pets::Pet>(m, "Dog")
    .def(py::init<std::string>());
```

```cpp
// cats.cpp, in a completely separate project from the above dogs.cpp.

// Binding for external library class:
py::class_<pets::Pet>(m, "Pet")
    .def("get_name", &pets::Pet::name);

// Binding for local extending class:
py::class_<Cat, pets::Pet>(m, "Cat")
    .def(py::init<std::string>());
```

```python
>>> import cats
>>> import dogs
Traceback (most recent call last):
  File "<stdin>", line 1, in <module>
ImportError: generic_type: type "Pet" is already registered!
```

为避开这点，你可以想py::class_传递py::module_local()属性，将外部类绑定到模块内部。

```cpp
// Pet binding in dogs.cpp:
py::class_<pets::Pet>(m, "Pet", py::module_local())
    .def("name", &pets::Pet::name);

// Pet binding in cats.cpp:
py::class_<pets::Pet>(m, "Pet", py::module_local())
    .def("get_name", &pets::Pet::name);
```

这样，Python侧的dogs.Pet和cats.Pet就是两个不同的类，两个模块也能顺利地同时导入，互不干扰。
有两点需要注意的是：

1. 外部模块不能返回或转换Pet示例到Python（除非他们也提供自己内部的绑定）；
2. 在Python的视角来看，他们就是两个截然不同的类。

注意，这个局部性仅作用于C++到Python方向。传递一个py::module_local类型到C++函数，在module-local类看来仍是合理的。
这意味着，下面的函数添加到任意哪个模块（不限于cats和dogs两个模块），它将可以通过dogs.Pet或cats.Pet参数来调用。

```cpp
m.def("pet_name", [](const pets::Pet &pet) { return pet.name(); });
```

举个例子，假设上述函数被添加到cats.cpp，dogs.cpp和frogs.cpp（frogs.cpp没有绑定Pets类）。

```python
>>> import cats, dogs, frogs  # No error because of the added py::module_local()
>>> mycat, mydog = cats.Cat("Fluffy"), dogs.Dog("Rover")
>>> (cats.pet_name(mycat), dogs.pet_name(mydog))
('Fluffy', 'Rover')
>>> (cats.pet_name(mydog), dogs.pet_name(mycat), frogs.pet_name(mycat))
('Rover', 'Fluffy', 'Fluffy')
```

即便其他模块已经全局地注册了相同的类型，我们还是可以使用py::module_local()来注册到另一个模块：
在module-local定义的模块，所有C++势力将被转为关联的Python类型。在其他模块，这个实例则被转为全局地Python类型。
