### 继承与向下转型

现在有两个具有继承关系的类：
```c++
struct Pet {
    Pet(const std::string &name) : name(name) { }
    std::string name;
};

struct Dog : Pet {
    Dog(const std::string &name) : Pet(name) { }
    std::string bark() const { return "woof!"; }
};
```

pybind11提供了两种方法来指明继承关系：1）将C++基类作为派生类`class_`的模板参数；2）将基类名作为`class_`的参数绑定到派生类。两种方法是等效的。

```c++
py::class_<Pet>(m, "Pet")
   .def(py::init<const std::string &>())
   .def_readwrite("name", &Pet::name);

// Method 1: template parameter:
py::class_<Dog, Pet /* <- specify C++ parent type */>(m, "Dog")
    .def(py::init<const std::string &>())
    .def("bark", &Dog::bark);

// Method 2: pass parent class_ object:
py::class_<Dog>(m, "Dog", pet /* <- specify Python parent type */)
    .def(py::init<const std::string &>())
    .def("bark", &Dog::bark);
```

指明继承关系后，派生类实例将获得两者的字段和方法：
```python
>>> p = example.Dog("Molly")
>>> p.name
u'Molly'
>>> p.bark()
u'woof!'
```

上面的例子是一个常规非多态的继承关系，表现在Python就是：
```c++
// 返回一个指向派生类的基类指针
m.def("pet_store", []() { return std::unique_ptr<Pet>(new Dog("Molly")); });
```

```python
>>> p = example.pet_store()
>>> type(p)  # `Dog` instance behind `Pet` pointer
Pet          # no pointer downcasting for regular non-polymorphic types
>>> p.bark()
AttributeError: 'Pet' object has no attribute 'bark'
```

`pet_store`函数返回了一个Dog实例，但由于基类并非多态类型，Python只识别到了Pet。在C++中，一个类至少有一个虚函数才会被视为多态类型。pybind11会自动识别这种多态机制。

```c++
struct PolymorphicPet {
    virtual ~PolymorphicPet() = default;
};

struct PolymorphicDog : PolymorphicPet {
    std::string bark() const { return "woof!"; }
};

// Same binding code
py::class_<PolymorphicPet>(m, "PolymorphicPet");
py::class_<PolymorphicDog, PolymorphicPet>(m, "PolymorphicDog")
    .def(py::init<>())
    .def("bark", &PolymorphicDog::bark);

// Again, return a base pointer to a derived instance
m.def("pet_store2", []() { return std::unique_ptr<PolymorphicPet>(new PolymorphicDog); });
```

```python
>>> p = example.pet_store2()
>>> type(p)
PolymorphicDog  # automatically downcast
>>> p.bark()
u'woof!'
```

pybind11会自动地将一个指向多态基类的指针，向下转型为实际的派生类类型。这和C++常见的情况不同，我们不仅可以访问基类的虚函数，还能获取到通过基类看不到的，具体的派生类的方法和属性。

