### 处理Python C API的错误

尽可能地使用pybind11 wrappers代替直接调用Python C API。
如果确实需要直接使用Python C API，除了需要手动管理引用计数外，还必须遵守pybind11的错误处理协议。

在调用Python C API后，如果Python返回错误，需要调用throw py::error_already_set();语句，
让pybind11来处理异常并传递给Python解释器。这包括对错误设置函数的调用，如PyErr_SetString。

```cpp
PyErr_SetString(PyExc_TypeError, "C API type error demo");
throw py::error_already_set();

// But it would be easier to simply...
throw py::type_error("pybind11 wrapper type error");
```

也可以调用PyErr_Clear来忽略错误。

任何Python错误必须被抛出或清除，否则Python/pybind11将处于无效的状态。
