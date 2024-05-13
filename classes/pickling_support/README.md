### 支持pickle

Python的pickle模块提供了强大的将Python对象图到二进制数据流的序列化和反序列化的功能。
pybind11也提供了py::pickle()定义来支持pickle和unpickle C++类。现在有这样一个类：

```cpp
class Pickleable {
public:
    Pickleable(const std::string &value) : m_value(value) { }
    const std::string &value() const { return m_value; }

    void setExtra(int extra) { m_extra = extra; }
    int extra() const { return m_extra; }
private:
    std::string m_value;
    int m_extra = 0;
};
```

Python中通过定义`__setstate__`和`__getstate__`使能pciking支持。
对于pybind11类，可以使用py::pickle()来绑定这两个函数：

```cpp
py::class_<Pickleable>(m, "Pickleable")
    .def(py::init<std::string>())
    .def("value", &Pickleable::value)
    .def("extra", &Pickleable::extra)
    .def("setExtra", &Pickleable::setExtra)
    .def(py::pickle(
        [](const Pickleable &p) { // __getstate__
            /* Return a tuple that fully encodes the state of the object */
            return py::make_tuple(p.value(), p.extra());
        },
        [](py::tuple t) { // __setstate__
            if (t.size() != 2)
                throw std::runtime_error("Invalid state!");

            /* Create a new C++ instance */
            Pickleable p(t[0].cast<std::string>());

            /* Assign any additional state */
            p.setExtra(t[1].cast<int>());

            return p;
        }
    ));
```

py::pickle()中的__setstate__部分遵循与py::init()单参数版本相同的规则，返回值可以是一个值，指针或者holder type。

Python中使用示例如下：

```python
try:
    import cPickle as pickle  # Use cPickle on Python 2.7
except ImportError:
    import pickle

p = Pickleable("test_value")
p.setExtra(15)
data = pickle.dumps(p, 2)
```

