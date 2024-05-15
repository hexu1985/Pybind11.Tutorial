### std::shared_ptr

class_可以传递一个表示持有者类型的模板类型，它用于管理对象的引用。
在不指定的情况下，默认为std::unique_ptr<Type>类型，这意味着当Python的引用计数为0时，将析构对象。
该模板类型可以指定为其他的智能指针或引用计数包装类，像下面我们就使用了std::shared_ptr：

```cpp
py::class_<Example, std::shared_ptr<Example> /* <- holder type */> obj(m, "Example");
```

注意，每个类仅能与一个持有者类型关联。

使用持有者类型的一个潜在的障碍就是，你需要始终如一的使用它们。猜猜下面的绑定代码有什么问题？

```cpp
class Child { };

class Parent {
public:
    Parent() : child(std::make_shared<Child>()) { }
    Child *get_child() { return child.get(); }  /* Hint: ** DON'T DO THIS ** */
private:
    std::shared_ptr<Child> child;
};

PYBIND11_MODULE(example, m) {
    py::class_<Child, std::shared_ptr<Child>>(m, "Child");

    py::class_<Parent, std::shared_ptr<Parent>>(m, "Parent")
        .def(py::init<>())
        .def("get_child", &Parent::get_child);
}
```

下面的Python代码将导致未定义行为（类似段错误）。

```cpp
from example import Parent
print(Parent().get_child())
```

问题在于Parent::get_child()返回类Child实例的指针，但事实上这个经由std::shared_ptr<...>管理的实例，在传递原始指针时就丢失了。
这个例子中，pybind11将创建第二个独立的std::shared_ptr<...>声明指针的所有权。最后，对象将被free两次，因为两个shared指针没法知道彼此的存在。

有两种方法解决这个问题：

对于智能指针管理的类型，永远不要在函数如参数或返回值中使用原始指针。
换句话说，在任何需要使用该类型指针的地方，使用它们指定的持有者类型代替。
这个例子中get_child()可以这样修改：

```cpp
std::shared_ptr<Child> get_child() { return child; }
```

定义Child时指定std::enable_shared_from_this<T>作为基类。 
这将在Child的基础上增加一点信息，让pybind11认识到这里已经存在一个std::shared_ptr<...>，并与之交互。
修改示例如下：

```cpp
class Child : public std::enable_shared_from_this<Child> { };
```
