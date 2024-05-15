### std::unique_ptr

给定一个带Python绑定的类Example，我们可以像下面一样返回它的unique pointer智能指针实例：

```cpp
std::unique_ptr<Example> create_example() { return std::unique_ptr<Example>(new Example()); }

m.def("create_example", &create_example);
```

没其他需要特殊处理的地方。需要注意的是，虽然允许返回unique_ptr对象，但是将其作为函数入参是非法的。例如，pybind11不能处理下列函数签名。

```cpp
void do_something_with_example(std::unique_ptr<Example> ex) { ... }
```

上面的签名意味着Python需要放弃对象的所有权，并将其传递给该函数，这通常是不可能的（对象可能在别处被引用）。
