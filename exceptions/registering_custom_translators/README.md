### 注册自定义异常翻译

如果上述默认异常转换策略不够用，pybind11也提供了注册自定义异常翻译的支持。
类似于pybind11 class，异常翻译也可以定义在模块内或global。
要注册一个使用C++异常的what()方法将C++到Python的异常转换，可以使用下面的方法：

```cpp
py::register_exception<CppExp>(module, "PyExp");
```

这个调用在指定模块创建了一个名称为PyExp的Python异常，并自动将CppExp相关的异常转换为PyExp异常。

相似的函数可以注册模块内的异常翻译：

```cpp
py::register_local_exception<CppExp>(module, "PyExp");
```

方法的第三个参数handle可以指定异常的基类：

```cpp
py::register_exception<CppExp>(module, "PyExp", PyExc_RuntimeError);
py::register_local_exception<CppExp>(module, "PyExp", PyExc_RuntimeError);
```

这样，PyExp异常可以捕获PyExp和RuntimeError。

Python内置的异常类型可以参考Python文档[Standard Exceptions](https://docs.python.org/3/c-api/exceptions.html#standard-exceptions)，
默认的基类为PyExc_Exception。

py::register_exception_translator(translator) 和py::register_local_exception_translator(translator) 提供了更高级的异常翻译功能，
它可以注册任意的异常类型。函数接受一个无状态的回调函数void(std::exception_ptr)。

C++异常抛出时，注册的异常翻译类将以注册时相反的顺序匹配，优先匹配模块内翻译类，然后再是全局翻译类。


