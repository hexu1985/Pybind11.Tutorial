### 成员变量

使用`class_::def_readwrite`方法可以导出公有成员变量，使用`class_::def_readonly`方法则可以导出只读成员。

```c++
py::class_<Pet>(m, "Pet")
    .def(py::init<const std::string &>())
    .def_readwrite("name", &Pet::name)
    // ... remainder ...
```

Python中使用示例如下：
```python
>>> p = example.Pet("Molly")
>>> p.name
u'Molly'
>>> p.name = "Charly"
>>> p.name
u'Charly'
```

假设`Pet::name`是一个私有成员变量，向外提供setter和getters方法。

```c++
class Pet {
public:
    Pet(const std::string &name) : name(name) { }
    void setName(const std::string &name_) { name = name_; }
    const std::string &getName() const { return name; }
private:
    std::string name;
};
```

可以使用`class_::def_property()`(只读成员使用`class_::def_property_readonly()`)来定义并私有成员，并生成相应的setter和geter方法：
```c++
py::class_<Pet>(m, "Pet")
    .def(py::init<const std::string &>())
    .def_property("name", &Pet::getName, &Pet::setName)
    // ... remainder ...
```

只写属性通过将read函数定义为nullptr来实现。

> **see also**: 相似的方法`class_::def_readwrite_static()`, `class_::def_readonly_static()` `class_::def_property_static()`, `class_::def_property_readonly_static()`用于绑定静态变量和属性。


