### 虚函数与继承

综合考虑虚函数与继承时，你需要为每个你允许在Python派生类中重载的方法提供重载方式。下面我们扩展Animal和Dog来举例：

```cpp
class Animal {
public:
    virtual std::string go(int n_times) = 0;
    virtual std::string name() { return "unknown"; }
};
class Dog : public Animal {
public:
    std::string go(int n_times) override {
        std::string result;
        for (int i=0; i<n_times; ++i)
            result += bark() + " ";
        return result;
    }
    virtual std::string bark() { return "woof!"; }
};
```

上节涉及到的Animal辅助类仍是必须的，为了让Python代码能够继承Dog类，我们也需要为Dog类增加一个跳板类，
来实现bark()和继承自Animal的go()、name()等重载方法（即便Dog类并不直接重载name方法）。

```cpp
class PyAnimal : public Animal {
public:
    using Animal::Animal; // Inherit constructors
    std::string go(int n_times) override { PYBIND11_OVERRIDE_PURE(std::string, Animal, go, n_times); }
    std::string name() override { PYBIND11_OVERRIDE(std::string, Animal, name, ); }
};
class PyDog : public Dog {
public:
    using Dog::Dog; // Inherit constructors
    std::string go(int n_times) override { PYBIND11_OVERRIDE(std::string, Dog, go, n_times); }
    std::string name() override { PYBIND11_OVERRIDE(std::string, Dog, name, ); }
    std::string bark() override { PYBIND11_OVERRIDE(std::string, Dog, bark, ); }
};
```

> 注意到name()和bark()尾部的逗号，这用来说明辅助类的函数不带任何参数。当函数至少有一个参数时，应该省略尾部的逗号。

注册一个继承已经在pybind11中注册的带虚函数的类，同样需要为其添加辅助类，即便它没有定义或重载任何虚函数：

```cpp
class Husky : public Dog {};
class PyHusky : public Husky {
public:
    using Husky::Husky; // Inherit constructors
    std::string go(int n_times) override { PYBIND11_OVERRIDE_PURE(std::string, Husky, go, n_times); }
    std::string name() override { PYBIND11_OVERRIDE(std::string, Husky, name, ); }
    std::string bark() override { PYBIND11_OVERRIDE(std::string, Husky, bark, ); }
};
```

我们可以使用模板辅助类将简化这类重复的绑定工作，这对有多个虚函数的基类尤其有用：

```cpp
template <class AnimalBase = Animal> class PyAnimal : public AnimalBase {
public:
    using AnimalBase::AnimalBase; // Inherit constructors
    std::string go(int n_times) override { PYBIND11_OVERRIDE_PURE(std::string, AnimalBase, go, n_times); }
    std::string name() override { PYBIND11_OVERRIDE(std::string, AnimalBase, name, ); }
};
template <class DogBase = Dog> class PyDog : public PyAnimal<DogBase> {
public:
    using PyAnimal<DogBase>::PyAnimal; // Inherit constructors
    // Override PyAnimal's pure virtual go() with a non-pure one:
    std::string go(int n_times) override { PYBIND11_OVERRIDE(std::string, DogBase, go, n_times); }
    std::string bark() override { PYBIND11_OVERRIDE(std::string, DogBase, bark, ); }
};
```

这样，我们只需要一个辅助方法来定义虚函数和纯虚函数的重载了。只是这样编译器就需要生成许多额外的方法和类。

下面我们在pybind11中注册这些类：

```cpp
PYBIND11_MODULE(example, m) {
    py::class_<Animal, PyAnimal<>> animal(m, "Animal");
    py::class_<Dog, Animal, PyDog<>> dog(m, "Dog");
    py::class_<Husky, Dog, PyDog<Husky>> husky(m, "Husky");

    animal.def(py::init<>())
          .def("go", &Animal::go)
          .def("name", &Animal::name);

    dog.def(py::init<>())
       .def("bark", &Dog::bark);

    husky.def(py::init<>());
}
```

注意，Husky不需要一个专门的辅助类，因为它没定义任何新的虚函数和纯虚函数的重载。

Python中的使用示例：

```python
class ShihTzu(Dog):
    def bark(self):
        return "yip!"
```

