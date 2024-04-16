### 附加的调用策略

除了以上的返回值策略外，进一步指定调用策略可以表明参数间的依赖关系，确保函数调用的稳定性。

**保活（keep alive）**

当一个C++容器对象包含另一个C++对象时，我们需要使用该策略。`keep_alive<Nurse, Patient>`表明至少在索引Nurse被回收前，索引Patient应该被保活。0表示返回值，1及以上表示参数索引。1表示隐含的参数this指针，而常规参数索引从2开始。当Nurse的值在运行前被检测到为None时，调用策略将什么都不做。

当nurse不是一个pybind11注册类型时，实现依赖于创建对nurse对象弱引用的能力。如果nurse对象不是pybind11注册类型，也不支持弱引用，程序将会抛出异常。

如果你使用一个错误的参数索引，程序将会抛出"Could not cativate keep_alive!"警告的运行时异常。这时，你应该review你代码中使用的索引。

参见下面的例子：一个list append操作，将新添加元素的生命周期绑定到添加的容器对象上：
```c++
py::class_<List>(m, "List").def("append", &List::append, py::keep_alive<1, 2>());
```

为了一致性，构造函数的实参索引也是相同的。索引1仍表示this指针，索引0表示返回值（构造函数的返回值被认为是void）。下面的示例将构造函数入参的生命周期绑定到被构造对象上。
```c++
py::class_<Nurse>(m, "Nurse").def(py::init<Patient &>(), py::keep_alive<1, 2>());
```

> Note: `keep_alive`与Boost.Python中的`with_custodian_and_ward`和`with_custodian_and_ward_postcall`相似。

**Call guard**

`call_guard<T>`策略允许任意T类型的scope guard应用于整个函数调用。示例如下：
```c++
m.def("foo", foo, py::call_guard<T>());
```

上面的代码等价于：
```c++
m.def("foo", [](args...) {
    T scope_guard;
    return foo(args...); // forwarded arguments
});
```

仅要求模板参数T是可构造的，如`gil_scoped_release`就是一个非常有用的类型。

`call_guard`支持同时制定多个模板参数，`call_guard<T1, T2, T3 ...>`。构造顺序是从左至右，析构顺序则相反。

> See also: `test/test_call_policies.cpp`含有更丰富的示例来展示`keep_alive`和`call_guard`的用法。


