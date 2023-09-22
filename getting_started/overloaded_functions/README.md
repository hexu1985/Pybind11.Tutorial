### 重载函数

重载方法即拥有相同的函数名，但入参不一样的函数。


在绑定重载函数时，我们需要增加函数签名相关的信息以消除歧义。绑定多个函数到同一个Python名称，将会自动创建函数重载链。Python将会依次匹配，找到最合适的重载函数。

```c++
m.def("add", static_cast<int(*)(int, int)>(&add), "A function which adds two int numbers");
m.def("add", static_cast<double(*)(double, double)>(&add), "A function which adds two double numbers");
```

如果你的编译器支持C++14，也可以使用下面的语法来转换重载函数：
```c++
m.def("add", py::overload_cast<int, int>(&add), "A function which adds two int numbers");
m.def("add", py::overload_cast<double, double>(&add), "A function which adds two double numbers");
```

这里，`py::overload_cast`仅需指定函数类型，不用给出返回值类型，以避免原语法带来的不必要的干扰(`void (Pet::*)`)。如果是基于const的重载，需要使用`py::const`标识。

