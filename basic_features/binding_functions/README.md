### 绑定函数

```cpp
#include <pybind11/pybind11.h>

int add(int i, int j) {
    return i + j;
}

PYBIND11_MODULE(example, m) {
    m.doc() = "pybind11 example plugin"; // optional module docstring
    m.def("add", &add, "A function which adds two numbers");
}
```

宏`PYBIND11_MODULE`会创建模块初始化函数，它在Python中`import`模块时被调用。
其参数分别是模块名，类型为`py::module_`的变量（m），是创建绑定的主要接口。
`module_::def()`方法，可以生成函数的绑定。


python测试代码：
```python
$ python
>>> import example
>>> example.add(1, 2)
3L
```

