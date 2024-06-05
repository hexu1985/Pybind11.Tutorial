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

在转换器中，应该在 try 块中使用 std::rethrow_exception 来重新抛出异常。
我们用一个或多个 catch 块来捕获适当的异常，而每个 catch 块应该使用 py::set_error() 来设置Python异常。

要声明自定义 Python 异常类型，请声明一个 py::exception 变量，并在关联的异常转换器中使用它（注意：在 lambda 表达式中使用它时，将其设置为静态声明通常很有用，而无需捕获）。

以下示例演示了两个假定的异常类 MyCustomException 和 OtherException ：第一个转换为自定义 python 异常 MyCustomError，而第二个转换为标准 python RuntimeError：

```cpp
PYBIND11_CONSTINIT static py::gil_safe_call_once_and_store<py::object> exc_storage;

exc_storage.call_once_and_store_result(
    [&]() { return py::exception<MyCustomException>(m, "MyCustomError"); });

py::register_exception_translator([](std::exception_ptr p) {
    try {
        if (p) std::rethrow_exception(p);
    } catch (const MyCustomException &e) {
        py::set_error(exc_storage.get_stored(), e.what());
    } catch (const OtherException &e) {
        py::set_error(PyExc_RuntimeError, e.what());
    }
});
```

单个转换器可以处理多个异常，如上面的示例所示。如果当前转换器中未 catch 这个异常，则先前注册的转换器将有机会继续处理。

如果所有已注册的异常转换器都无法处理异常，则由默认转换器处理异常。

> 为自定义异常转换器中捕获的每个异常调用 py::set_error()。否则将导致 Python 崩溃，并显示 SystemError: error return without exception set。
> 您不打算处理的异常不应该被捕获，或者可以显式（重新）抛出，以将其委托给其他先前声明的异常转换器。

