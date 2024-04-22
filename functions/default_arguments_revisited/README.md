### 再探默认参数

前面的章节已经讨论了默认参数的基本用法。关于实现有一个值得关注的点，就是默认参数在声明时就被转换为Python对象了。看看下面的例子：

```cpp
py::class_<MyClass>("MyClass").def("myFunction", py::arg("arg") = SomeType(123));
```

这个例子里，必须保证SomeType类型已经被binding了（通过py::class_），不然就会抛出异常。

另一个值得注意的事情就是，生成的函数签名将使用对象的__repr__方法来处理默认参数值。如果对象没有提供该方法，那么函数签名将不能直观的看出默认参数值。

```py
FUNCTIONS

|  myFunction(...)
|      Signature : (MyClass, arg : SomeType = <SomeType object at 0x101b7b080>) -> NoneType
```

要处理这个问题，我们需要定义SomeType.__repr__方法，或者使用arg_v给默认参数手动添加方便阅读的注释。

```cpp
py::class_<MyClass>("MyClass")
    .def("myFunction", py::arg_v("arg", SomeType(123), "SomeType(123)"));
```

有时，可能需要使用空指针作为默认参数：

```cpp
py::class_<MyClass>("MyClass")
    .def("myFunction", py::arg("arg") = static_cast<SomeType *>(nullptr));
```
