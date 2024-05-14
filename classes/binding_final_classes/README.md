### 绑定final类

在C++11中，我们可以使用findal关键字来确保一个类不被继承。
py::is_final属性则可以用来确保一个类在Python中不被继承。底层的C++类型不需要定义为final。

```cpp
class IsFinal final {};

py::class_<IsFinal>(m, "IsFinal", py::is_final());
```

在Python中试图继承这个类，将导致错误：

```python
class PyFinalChild(IsFinal):
    pass

TypeError: type 'IsFinal' is not an acceptable base type
```

