### 使用模板参数绑定函数

你可以绑定具有模板参数的函数。下面是一个函数：

```cpp
template <typename T>
void set(T t);
```

C++ 模板无法在运行时实例化，因此无法绑定非实例化函数：

```cpp
// BROKEN (this will not compile)
m.def("set", &set);
```

你必须单独绑定每个实例化的函数模板。您可以使用相同的名称绑定每个实例化，这将被视为重载函数：

```cpp
m.def("set", &set<int>);
m.def("set", &set<std::string>);
```

有时，将它们与单独的名称绑定起来会更清楚，这也是一种选择：

```cpp
m.def("setInt", &set<int>);
m.def("setString", &set<std::string>);
```
