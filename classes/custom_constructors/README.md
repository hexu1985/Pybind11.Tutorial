### 定制构造函数

前面章节介绍了绑定构造函数的方法，但它仅仅在C++侧刚好有对应的函数时才能正常工作。
为了扩展到更通用的情况，pybind11可以绑定工厂方法作为构造函数。如下所示：

```cpp
class Example {
private:
    Example(int); // private constructor
public:
    // Factory function:
    static Example create(int a) { return Example(a); }
};

py::class_<Example>(m, "Example")
    .def(py::init(&Example::create));
```

虽然可以直接绑定create方法，有时将其在Python侧将其作为构造函数公开更为合适。
这可以通过调用.def(py::init(...))来完成，只需将对应的函数（返回一个新实例，如create）作为参数传入py::init()即可。
同样的，用这个方法我们也可以传入一个函数，它返回新实例的原始指针或持有者（如`std::unique_ptr`）。如下所示：

```cpp
class Example {
private:
    Example(int); // private constructor
public:
    // Factory function - returned by value:
    static Example create(int a) { return Example(a); }

    // These constructors are publicly callable:
    Example(double);
    Example(int, int);
    Example(std::string);
};

py::class_<Example>(m, "Example")
    // Bind the factory function as a constructor:
    .def(py::init(&Example::create))
    // Bind a lambda function returning a pointer wrapped in a holder:
    .def(py::init([](std::string arg) {
        return std::unique_ptr<Example>(new Example(arg));
    }))
    // Return a raw pointer:
    .def(py::init([](int a, int b) { return new Example(a, b); }))
    // You can mix the above with regular C++ constructor bindings as well:
    .def(py::init<double>())
    ;
```

当Python侧调用这些构造函数时，pybind11将调用工厂函数，并将返回的C++示例存储到Python实例中。

当与重载函数跳板类结合使用时，有两种方法。第一种方法是跳板类增加一个构造函数，函数接受原类的右值引用，
这样我们可以从原类的工厂函数构造跳板类的实例。第二种方法是使用py::init()提供原类和跳板类两个工厂函数。

你也可以指定一个工厂函数，它总是返回跳板类的实例，这与`py::init_alias<...>`的行为类似。

下面的示例展示了这两种方法：

```cpp
#include <pybind11/factory.h>
class Example {
public:
    // ...
    virtual ~Example() = default;
};
class PyExample : public Example {
public:
    using Example::Example;
    PyExample(Example &&base) : Example(std::move(base)) {}
};
py::class_<Example, PyExample>(m, "Example")
    // Returns an Example pointer.  If a PyExample is needed, the Example
    // instance will be moved via the extra constructor in PyExample, above.
    .def(py::init([]() { return new Example(); }))
    // Two callbacks:
    .def(py::init([]() { return new Example(); } /* no alias needed */,
                  []() { return new PyExample(); } /* alias needed */))
    // *Always* returns an alias instance (like py::init_alias<>())
    .def(py::init([]() { return new PyExample(); }))
    ;
```

**大括号初始化**

pybind11潜在地使用C++11的大括号初始化来调用目标类的构造函数，这意味着它也可以绑定隐式的构造函数：

```cpp
struct Aggregate {
    int a;
    std::string b;
};

py::class_<Aggregate>(m, "Aggregate")
    .def(py::init<int, const std::string &>());
```

> Note: 大括号初始化优先匹配带列表初始化的重载构造函数。极少数情况下会出问题，
> 你可以使用py::init(...)传入一个构造新对象的匿名函数来处理这个问题。
