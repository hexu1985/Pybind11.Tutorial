### 允许/禁止空参数

当函数接受由`py::class_`注册的C++类型的指针或shared holder(如指针指针等)，
pybind11允许将Python的None传递给函数，等同于C++中传递nullptr给函数。

我们可以使用py::arg对象的.none方法来显式地使能或禁止该行为。

```cpp
py::class_<Dog>(m, "Dog").def(py::init<>());
py::class_<Cat>(m, "Cat").def(py::init<>());
m.def("bark", [](Dog *dog) -> std::string {
    if (dog) return "woof!"; /* Called with a Dog instance */
    else return "(no dog)"; /* Called with None, dog == nullptr */
}, py::arg("dog").none(true));
m.def("meow", [](Cat *cat) -> std::string {
    // Can't be called with None argument
    return "meow";
}, py::arg("cat").none(false));
```

这样，Python调用bark(None)将返回"(no dog)"，调用meow(None)将抛出异常TypeError。

```python
>>> from animals import Dog, Cat, bark, meow
>>> bark(Dog())
'woof!'
>>> meow(Cat())
'meow'
>>> bark(None)
'(no dog)'
>>> meow(None)
Traceback (most recent call last):
  File "<stdin>", line 1, in <module>
TypeError: meow(): incompatible function arguments. The following argument types are supported:
    1. (cat: animals.Cat) -> str

Invoked with: None
```

在不显式指定的情况下，默认支持传递None。

> Note: 即使为参数指定了 .none(true)，也只会将自定义类型和不透明类型转换为 nullptr。
> 指向内置类型（double *、int *、...）和 STL 类型（std::vector<T> *、...;如果包含 pybind11/stl.h）
> 的指针在转换为 C++ 时会被复制，并且不允许 None 作为参数。
> 若要传递这些复制类型的可选参数，请考虑使用 std::optional<T>
> 

