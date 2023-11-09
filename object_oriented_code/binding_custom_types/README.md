### 创建一个自定义类型的绑定

让我们来看一个更加复杂的例子：绑定一个C++自定义数据结构`Pet`。定义如下：

```cpp
struct Pet {
    Pet(const std::string &name) : name(name) { }
    void setName(const std::string &name_) { name = name_; }
    const std::string &getName() const { return name; }

    std::string name;
};
```

绑定代码如下所示：

```cpp
#include <pybind11/pybind11.h>
namespace py = pybind11;

PYBIND11_MODULE(example, m) {
    py::class_<Pet>(m, "Pet")
        .def(py::init<const std::string &>())
        .def("setName", &Pet::setName)
        .def("getName", &Pet::getName);
}
```

`class_`会创建C++ class或 struct的绑定。`init()`方法使用类构造函数的参数类型作为模板参数，并包装相应的构造函数（详见[自定义构造函数](https://pybind11.readthedocs.io/en/stable/advanced/classes.html#custom-constructors)）。Python使用示例如下；

```python
>>> import example
>>> p = example.Pet("Molly")
>>> print(p)
<example.Pet object at 0x10cd98060>
>>> p.getName()
u'Molly'
>>> p.setName("Charly")
>>> p.getName()
u'Charly'
```

> **See also**：静态成员函数需要使用`class_::def_static`来绑定。
