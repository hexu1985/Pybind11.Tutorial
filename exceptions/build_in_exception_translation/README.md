### C++内置异常到Python异常的转换

当Python通过pybind11调用C++代码时，pybind11将捕获C++异常，并将其翻译为对应的Python异常后抛出。
这样Python代码就能够处理它们。

pybind11定义了std::exception及其标准子类，和一些特殊异常到Python异常的翻译。
由于它们不是真正的Python异常，所以不能使用Python C API来检查。
相反，它们是纯C++异常，当它们到达异常处理器时，pybind11将其翻译为对应的Python异常。

| Exception thrown by C++ | Translated to Python exception type |
| ----------------------- | ----------------------------------- |
| std::exception | RuntimeError |
| std::bad_alloc | MemoryError |
| std::domain_error | ValueError |
| std::invalid_argument | ValueError |
| std::length_error | ValueError |
| std::out_of_range | IndexError |
| std::range_error | ValueError |
| std::overflow_error | OverflowError |
| pybind11::stop_iteration | StopIteration (used to implement custom iterators) |
| pybind11::index_error | IndexError (used to indicate out of bounds access in `__getitem__`, `__setitem__`, etc.) |
| pybind11::key_error | KeyError (used to indicate out of bounds access in `__getitem__`, `__setitem__` in dict-like objects, etc.) |
| pybind11::value_error | ValueError (used to indicate wrong value passed in container.remove(...)) |
| pybind11::type_error | TypeError |
| pybind11::buffer_error | BufferError |
| pybind11::import_error | ImportError |
| pybind11::attribute_error | AttributeError |
| Any other exception | RuntimeError |

异常翻译不是双向的。即上述异常不会捕获源自Python的异常。Python的异常，需要捕获pybind11::error_already_set。

这里有个特殊的异常，当入参不能转化为Python对象时，handle::call()将抛出cast_error异常。
