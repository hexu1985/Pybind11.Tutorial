### 函数对象

要使能以下属性，需要包含pybind11/functional.h

C++11引入了lambda函数和泛型多态函数包装器`std::function<>`，这为使用函数提供了强大的新方法。
lambda函数有两种类型：无状态的lambda函数像经典的函数指针，链接到一段匿名的代码段，
而有状态 lambda 函数还依赖于存储在匿名 lambda 闭包对象中的捕获变量。

下面是一个 C++ 函数的简单示例，该函数将签名为 `int -> int` 的任意函数（有状态或无状态）作为参数，并以值 10 调用函数f。

```cpp
int func_arg(const std::function<int(int)> &f) {
    return f(10);
}
```

下面的示例涉及更多：它接受一个签名 `int -> int` 的函数，并返回另一个相同类型的函数。
返回值是一个有状态的 lambda 函数，它将函数对象 f 存储在捕获对象中，并在执行时将 1 添加到其返回值中。

```cpp
std::function<int(int)> func_ret(const std::function<int(int)> &f) {
    return [f](int i) {
        return f(i) + 1;
    };
}
```

在C++回调函数中使用python具名参数，需要使用py::cpp_function包裹，类似于下面的代码：

```cpp
py::cpp_function func_cpp() {
    return py::cpp_function([](int i) { return i+1; },
       py::arg("number"));
}
```

包含pybind11/functional.h头文件后，可以简单地直接为上述函数生成绑定代码：

```cpp
#include <pybind11/functional.h>

PYBIND11_MODULE(example, m) {
    m.def("func_arg", &func_arg);
    m.def("func_ret", &func_ret);
    m.def("func_cpp", &func_cpp);
}
```

Python中交互示例如下：

```python
$ python
>>> import example
>>> def square(i):
...     return i * i
...
>>> example.func_arg(square)
100L
>>> square_plus_1 = example.func_ret(square)
>>> square_plus_1(4)
17L
>>> plus_1 = func_cpp()
>>> plus_1(number=43)
44L
```

> Warning
> 请记住在从C++传递函数对象到Python的过程中（反向亦然），将生成一些包装代码来完成两种语言的函数调用。
> 这种翻译自然会稍微增加函数调用的开销。当一个函数在Python和C++之间来回拷贝多次时，包装层数会不断累积，会明显降低性能。
> 这里有个例外：一个无状态函数作为参数传递给在Python中公开的另一个C++函数时，将不会有额外的开销。
> Pybind11将从封装的函数中提取C++函数指针，以回避潜在地C++ -> Python -> C++的往返。

