### 动态属性

原生的Pyhton类可以动态地获取新属性：
```python
>>> class Pet:
...    name = "Molly"
...
>>> p = Pet()
>>> p.name = "Charly"  # overwrite existing
>>> p.age = 2  # dynamically add a new attribute
```

默认情况下，从C++导出的类不支持动态属性，其可写属性必须是通过`class_::def_readwrite`或`class_::def_property`定义的。试图设置其他属性将产生错误：
```python
>>> p = example.Pet()
>>> p.name = "Charly"  # OK, attribute defined in C++
>>> p.age = 2  # fail
AttributeError: 'Pet' object has no attribute 'age'
```

要让C++类也支持动态属性，我们需要在`py::class_`的构造函数添加`py::dynamic_attr`标识：
```c++
py::class_<Pet>(m, "Pet", py::dynamic_attr())
    .def(py::init<>())
    .def_readwrite("name", &Pet::name);
```

这样，之前报错的代码就能够正常运行了。

```python
>>> p = example.Pet()
>>> p.name = "Charly"  # OK, overwrite value in C++
>>> p.age = 2  # OK, dynamically add a new attribute
>>> p.__dict__  # just like a native Python class
{'age': 2}
```

需要提醒一下，支持动态属性会带来小小的运行时开销。不仅仅因为增加了额外的`__dict__`属性，还因为处理循环引用时需要花费更多的垃圾收集跟踪花销。但是不必担心这个问题，因为原生Python类也有同样的开销。默认情况下，pybind11导出的类比原生Python类效率更高，使能动态属性也只是让它们处于同等水平而已。

