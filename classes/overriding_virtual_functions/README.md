### 在Python中重载虚函数

假设有一个含有虚函数的C++类或接口，我们想在Python中重载虚函数。

```cpp
class Animal {
public:
    virtual ~Animal() { }
    virtual std::string go(int n_times) = 0;
};

class Dog : public Animal {
public:
    std::string go(int n_times) override {
        std::string result;
        for (int i=0; i<n_times; ++i)
            result += "woof! ";
        return result;
    }
};
```

现在有一个普通函数，它调用任意Animal实例的go()函数。

```cpp
std::string call_go(Animal *animal) {
    return animal->go(3);
}
```

pybind11绑定代码如下：

```cpp
PYBIND11_MODULE(example, m) {
    py::class_<Animal>(m, "Animal")
        .def("go", &Animal::go);

    py::class_<Dog, Animal>(m, "Dog")
        .def(py::init<>());

    m.def("call_go", &call_go);
}
```

但是，这样绑定不可扩展，当我们尝试继承Animal类时会提示"No constructor defined!"，因为Animal无法构造。
这时，我们需要类似于"跳板(trampoline)"的工具来重定向虚函数调用到Python中。

我们可以在Python中定义一个新的Animal类作为辅助跳板：

```cpp
class PyAnimal : public Animal {
public:
    /* Inherit the constructors */
    using Animal::Animal;

    /* Trampoline (need one for each virtual function) */
    std::string go(int n_times) override {
        PYBIND11_OVERRIDE_PURE(
            std::string, /* Return type */
            Animal,      /* Parent class */
            go,          /* Name of function in C++ (must match Python name) */
            n_times      /* Argument(s) */
        );
    }
};
```

定义纯虚函数时需要使用PYBIND11_OVERRIDE_PURE宏，而有默认实现的虚函数则使用PYBIND11_OVERRIDE。
PYBIND11_OVERRIDE_PURE_NAME 和PYBIND11_OVERRIDE_NAME 宏的功能类似，主要用于C函数名和Python函数名不一致的时候。
以__str__为例：

```cpp
std::string toString() override {
  PYBIND11_OVERRIDE_NAME(
      std::string, // Return type (ret_type)
      Animal,      // Parent class (cname)
      "__str__",   // Name of method in Python (name)
      toString,    // Name of function in C++ (fn)
  );
}
```

Animal类的绑定代码也需要一些微调：

```cpp
PYBIND11_MODULE(example, m) {
    py::class_<Animal, PyAnimal /* <--- trampoline*/>(m, "Animal")
        .def(py::init<>())
        .def("go", &Animal::go);

    py::class_<Dog, Animal>(m, "Dog")
        .def(py::init<>());

    m.def("call_go", &call_go);
}
```

pybind11通过向class_指定额外的模板参数PyAnimal，让我们可以在Python中继承Animal类。

接下来，我们可以像往常一样定义构造函数。绑定时我们需要使用真实类，而不是辅助类。

```cpp
py::class_<Animal, PyAnimal /* <--- trampoline*/>(m, "Animal");
    .def(py::init<>())
    .def("go", &PyAnimal::go); /* <--- THIS IS WRONG, use &Animal::go */
```

但是，上面的改动可以让我们在Python中继承Animal类，而不能继承Dog类。后续章节将会在此基础上进一步改进。

下面的Python代码展示了我们继承并重载了Animal::go方法，并通过虚函数来调用它：

```cpp
from example import *
d = Dog()
call_go(d)     # u'woof! woof! woof! '
class Cat(Animal):
    def go(self, n_times):
        return "meow! " * n_times

c = Cat()
call_go(c)   # u'meow! meow! meow! '
```

如果你在派生的Python类中自定义了一个构造函数，你必须保证显示调用C++构造函数(通过__init__)，不管它是否为默认构造函数。
否则，实例属于C++那部分的内存就未初始化，可能导致未定义行为。在pybind11 2.6版本中，这种错误将会抛出TypeError异常。

```cpp
class Dachshund(Dog):
    def __init__(self, name):
        Dog.__init__(self)  # Without this, a TypeError is raised.
        self.name = name

    def bark(self):
        return "yap!"
```

注意必须显式地调用__init__，而不应该使用supper()。在一些简单的线性继承中，supper()或许可以正常工作；
一旦你混合Python和C++类使用多重继承，由于Python MRO和C++的机制，一切都将崩溃。


