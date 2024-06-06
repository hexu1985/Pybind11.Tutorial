### 扩展跳板类的功能

**跳板类的初始化**

默认情况下，跳板类需要的时候才初始化，即当一个Python类继承了绑定的C++类时（而不是创建绑定类的实例时），
或者注册的构造函数仅对跳板类而非注册类有效时。
这主要是处于性能的考量：如果只有虚函数需要跳板类时，不初始化跳板类可以避免运行时检查Python继承类是否有重载函数，以提高性能。

有时，将跳板类作为一个不仅仅用于处理虚函数分发的中间类来初始化还是有用的。
例如，这个类可以执行额外的初始化操作，额外的析构操作，定义属性或方法来给类提供类似Python风格的接口。

要让pybind11在创建类实例时，总是初始化跳板类，类的构造函数需要使用`py::init_alias<Args, ...>()`来代替`py::init<Args, ...>()`。
这样可以强制通过跳板类来构造，确保类成员的初始化和析构。


**差异化函数签名**

有时，我们难以创建参数和返回类型间的一一映射关系。如C++的参数即是输入又是输出的情况（入参为引用，在函数中修改该参数）。

我们可以通过跳板类来解决这种Python方法输入和输出的问题，
也可以参考 [Limitations involving reference arguments](https://pybind11.readthedocs.io/en/stable/faq.html#faq-reference-arguments) 中的处理方法。

get_override()函数允许Python从跳板类方法中检索方法的实现。
例如，考虑一个具有签名 `bool myMethod（int32_t& value)` 的 C++ 方法，其中返回值指示是否应该对`value`进行某些修改。

在 Python 端，允许 Python 函数返回 None 或 int 来说更方便：

```cpp
bool MyClass::myMethod(int32_t& value)
{
    pybind11::gil_scoped_acquire gil;  // Acquire the GIL while in this scope.
    // Try to look up the overridden method on the Python side.
    pybind11::function override = pybind11::get_override(this, "myMethod");
    if (override) {  // method is found
        auto obj = override(value);  // Call the Python function.
        if (py::isinstance<py::int_>(obj)) {  // check if it returned a Python integer type
            value = obj.cast<int32_t>();  // Cast it and assign it to the value.
            return true;  // Return true; value should be used.
        } else {
            return false;  // Python returned none, return false.
        }
    }
    return false;  // Alternatively return MyClass::myMethod(value);
}
```
