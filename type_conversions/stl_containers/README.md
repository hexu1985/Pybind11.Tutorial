### STL容器

#### 自动转换

包含头文件pybind11/stl.h后，
自动支持 `std::vector<>`/`std::deque<>`/`std::list<>`/`std::array<>`/`std::valarray<>`, `std::set<>`/`std::unordered_set<>`, 
和`std::map<>`/`std::unordered_map<>` 到Python list, set 和 dict 的类型转换。 
`std::pair<>` 和`std::tuple<>` 类型转换在pybind11/pybind11.h中已经支持。

隐式转换的主要缺点就是Python和C++之间的容器类型转换都需要拷贝数据，这对程序语义和性能有一定的影响。

> Note: 这些类型任意嵌套都是可以的。

pybind11/stl.h支持C++17的 `std::optional<>` 和`std::variant<>`，C++14的`std::experimental::optional<>`。

---

#### 制作opaque类型

pybind11严重依赖于模板匹配机制来转换STL类型的参数和返回值，如vector，链表，哈希表等。
甚至会递归处理，如lists of hash maps of pairs of elementary and custom types。

然而，这个方法的一个基本限制就是，Python和C++类型的转换涉及到拷贝操作，这妨碍了pass-by-reference的语义。

假设我们绑定如下函数：

```cpp
void append_1(std::vector<int> &v) {
   v.push_back(1);
}
```

在Python中调用它：

```python
>>> v = [5, 6]
>>> append_1(v)
>>> print(v)
[5, 6]
```

如你所见，通过引用传递STL数据时，修改并不会传递到Python侧。相同的场景表现在通过def_readwrite或def_readonly函数公开STL数据结构时：

```cpp
/* ... definition ... */

class MyClass {
    std::vector<int> contents;
};

/* ... binding code ... */

py::class_<MyClass>(m, "MyClass")
    .def(py::init<>())
    .def_readwrite("contents", &MyClass::contents);
```

这个例子中，属性可以整体的读写。但是，涉及到append操作时就无能为力了：

```python
>>> m = MyClass()
>>> m.contents = [5, 6]
>>> print(m.contents)
[5, 6]
>>> m.contents.append(7)
>>> print(m.contents)
[5, 6]
```

如你所见，通过引用传递STL数据时，修改并不会传递到Python侧。相同的场景表现在通过def_readwrite或def_readonly函数公开STL数据结构时：

```cpp
/* ... definition ... */

class MyClass {
    std::vector<int> contents;
};

/* ... binding code ... */

py::class_<MyClass>(m, "MyClass")
    .def(py::init<>())
    .def_readwrite("contents", &MyClass::contents);
```

这个例子中，属性可以整体的读写。但是，涉及到append操作时就无能为力了：

```python
>>> m = MyClass()
>>> m.contents = [5, 6]
>>> print(m.contents)
[5, 6]
>>> m.contents.append(7)
>>> print(m.contents)
[5, 6]
```

最后，在处理大型列表时，涉及的拷贝操作会耗费巨大。为处理上述场景，pybind11提供了PYBIND11_MAKE_OPAQUE(T)来禁用基于模板的类型转换机制，从而使他们变得不透明（opaque）。
opaque对象的内容永远不会被检查或提取，因此它们可以通过引用传递。例如，要将`std::vector<int>`转换为opaque类型，需要在所有绑定代码之前添加声明：

```cpp
PYBIND11_MAKE_OPAQUE(std::vector<int>);
```

这个宏必须在顶层（所有命名空间外）设定，因为然添加了type_caster的模板实例化。
如果你绑定代码包含多个编译单元，需要在每个文件使用`std::vector<int>`前指定（通常通过公共的头文件来实现）。
opaque对象有相关的`class_`定义来联系Python中的类名，还需定义一组有用的操作，如：


```cpp
py::class_<std::vector<int>>(m, "IntVector")
    .def(py::init<>())
    .def("clear", &std::vector<int>::clear)
    .def("pop_back", &std::vector<int>::pop_back)
    .def("__len__", [](const std::vector<int> &v) { return v.size(); })
    .def("__iter__", [](std::vector<int> &v) {
       return py::make_iterator(v.begin(), v.end());
    }, py::keep_alive<0, 1>()) /* Keep vector alive while iterator is used */
    // ....
```

---

#### 绑定STL容器

公开STL容器作为一个Python对象时一个相当常见的需求，因此pybind11也提供了一个可选的头文件pybind11/stl_bind.h来做这件事。
映射容器会尽可能的去匹配Python对应类型的行为。下面示例将展示该机制的使用方法：

```cpp
// Don't forget this
#include <pybind11/stl_bind.h>

PYBIND11_MAKE_OPAQUE(std::vector<int>);
PYBIND11_MAKE_OPAQUE(std::map<std::string, double>);

// ...

// later in binding code:
py::bind_vector<std::vector<int>>(m, "VectorInt");
py::bind_map<std::map<std::string, double>>(m, "MapStringDouble");
```

绑定STL容器时，pybind11会根据容器元素的类型来决定该容器是否应该局限于模块内（参考Module-local class bindings特性）。
如果容器元素的类型不是已经绑定的自定义类型且未标识py::module_local，那么容器绑定将应用py::module_local。
这包括数值类型、strings、Eigen类型，和其他在绑定STL容器时还未绑定的类型。module-local绑定的意图是为了避免模块间的潜在的冲突（如，两个独立的模块都试图绑定`std::vector<int>`）。

通过指定属性py::module_local()或py_module_local(false)，你也可以强制绑定的STL容器公开在模块内或全局：

```cpp
py::bind_vector<std::vector<int>>(m, "VectorInt", py::module_local(false));
```

注意：这样的全局绑定可能会导致模块无法加载，如果其他的模块也绑定了相同的容器类型（如`std::vector<int>`）并以加载。
