### 以Python对象作为参数

pybind11通过简单的C++封装类，公开了绝大多数Python类型。这些封装类也可以在绑定代码宏作为函数参数使用，这样我们就可以在C++侧使用原生的python类型。举个遍历Python dict的例子：
```c++
void print_dict(const py::dict& dict) {
    /* Easily interact with Python types */
    for (auto item : dict)
        std::cout << "key=" << std::string(py::str(item.first)) << ", "
                  << "value=" << std::string(py::str(item.second)) << std::endl;
}

// it can be exported as follow:
m.def("print_dict", &print_dict);
```

在Python中使用如下：
```python
>>> print_dict({"foo": 123, "bar": "hello"})
key=foo, value=123
key=bar, value=hello
```


