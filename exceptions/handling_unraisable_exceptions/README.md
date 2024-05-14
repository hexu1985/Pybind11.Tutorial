### 处理unraiseable异常

如果Python调用的C++析构函数或任何标记为noexcept(true)的函数抛出了异常，该异常不会传播出去。
如果它们在调用图中抛出或捕捉不到任何异常，c++运行时将调用std::terminate()立即终止程序。

类似的，在类__del__方法引发的Python异常也不会传播，但被Python作为unraisable错误记录下来。
在Python 3.8+中，将触发system hook，并记录auditing event日志。

任何noexcept函数应该使用try-catch代码块来捕获error_already_set（或其他可能出现的异常）。
pybind11包装的Python异常并非真正的Python异常，它是pybind11捕获并转化的C++异常。
noexcept函数不能传播这些异常。我们可以将它们转换为Python异常，然后丢弃discard_as_unraisable，如下所示。

```cpp
void nonthrowing_func() noexcept(true) {
    try {
        // ...
    } catch (py::error_already_set &eas) {
        // Discard the Python error using Python APIs, using the C++ magic
        // variable __func__. Python already knows the type and value and of the
        // exception object.
        eas.discard_as_unraisable(__func__);
    } catch (const std::exception &e) {
        // Log and discard C++ exceptions.
        third_party::log(e);
    }
}
```
