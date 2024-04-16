### 返回值策略

Python和C++在管理内存和对象生命周期管理上存在本质的区别。这导致我们在创建返回no-trivial类型的函数绑定时会出问题。仅通过类型信息，我们无法明确是Python侧需要接管返回值并负责释放资源，还是应该由C++侧来处理。因此，pybind11提供了一些返回值策略来确定由哪方管理资源。这些策略通过`model::def()`和`class_def()`来指定，默认策略为`return_value_policy::automatic`。

返回值策略难以捉摸，正确地选择它们则显得尤为重要。下面我们通过一个简单的例子来阐释选择错误的情形：
```c++
/* Function declaration */
Data *get_data() { return _data; /* (pointer to a static data structure) */ }
...

/* Binding code */
m.def("get_data", &get_data); // <-- KABOOM, will cause crash when called from Python
```

当Python侧调用`get_data()`方法时，返回值（原生C++类型）必须被转换为合适的Python类型。在这个例子中，默认的返回值策略（`return_value_policy::automatic`）使得pybind11获取到了静态变量`_data`的所有权。

当Python垃圾收集器最终删除`_data`的Python封装时，pybind11将尝试删除C++实例（通过operator delete()）。这时，这个程序将以某种隐蔽的错误并涉及静默数据破坏的方式崩溃。

对于上面的例子，我们应该指定返回值策略为`return_value_policy::reference`，这样全局变量的实例仅仅被引用，而不涉及到所有权的转移：
```c++
m.def("get_data", &get_data, py::return_value_policy::reference);
```

另一方面，引用策略在多数其他场合并不是正确的策略，忽略所有权的归属可能导致资源泄漏。作为一个使用pybind11的开发者，熟悉不同的返回值策略及其适用场合尤为重要。下面的表格将提供所有策略的概览：

| 返回值策略                                 | 描述                                                         |
| ------------------------------------------ | ------------------------------------------------------------ |
| `return_value_policy::take_ownership`      | 引用现有对象（不创建一个新对象），并获取所有权。在引用计数为0时，Python将调用析构函数和delete操作销毁对象。 |
| `return_value_policy::copy`                | 拷贝返回值，这样Python将拥有拷贝的对象。该策略相对来说比较安全，因为两个实例的生命周期是分离的。 |
| `return_value_policy::move`                | 使用`std::move`来移动返回值的内容到新实例，新实例的所有权在Python。该策略相对来说比较安全，因为两个实例的生命周期是分离的。 |
| `return_value_policy::reference`           | 引用现有对象，但不拥有所有权。C++侧负责该对象的生命周期管理，并在对象不再被使用时负责析构它。注意：当Python侧还在使用引用的对象时，C++侧删除对象将导致未定义行为。 |
| `return_value_policy::reference_internal`  | 返回值的生命周期与父对象的生命周期相绑定，即被调用函数或属性的`this`或`self`对象。这种策略与reference策略类似，但附加了`keep_alive<0, 1>`调用策略保证返回值还被Python引用时，其父对象就不会被垃圾回收掉。这是由`def_property`、`def_readwrite`创建的属性getter方法的默认返回值策略。 |
| `return_value_policy::automatic`           | 当返回值是指针时，该策略使用`return_value_policy::take_ownership`。反之对左值和右值引用使用`return_value_policy::copy`。请参阅上面的描述，了解所有这些不同的策略的作用。这是`py::class_`封装类型的默认策略。 |
| `return_value_policy::automatic_reference` | 和上面一样，但是当返回值是指针时，使用`return_value_policy::reference`策略。这是在C++代码手动调用Python函数和使用`pybind11/stl.h`中的casters时的默认转换策略。你可能不需要显式地使用该策略。 |

返回值策略也可以应用于属性：
```c++
class_<MyClass>(m, "MyClass")
    .def_property("data", &MyClass::getData, &MyClass::setData,
                  py::return_value_policy::copy);
```

在技术层面，上述代码会将策略同时应用于getter和setter函数，但是setter函数并不关心返回值策略，这样做仅仅出于语法简洁的考虑。或者，你可以通过`cpp_function`构造函数来传递目标参数：
```c++
class_<MyClass>(m, "MyClass")
    .def_property("data"
        py::cpp_function(&MyClass::getData, py::return_value_policy::copy),
        py::cpp_function(&MyClass::setData)
    );
```

**注意**：代码使用无效的返回值策略将导致未初始化内存或多次free数据结构，这将导致难以调试的、不确定的问题和段错误。因此，花点时间来理解上面表格的各个选项是值得的。

**提示**：
1. 上述策略的另一个重点是，他们仅可以应用于pybind11还不知晓的实例，这时策略将澄清返回值的生命周期和所有权问题。当pybind11已经知晓参数（通过其在内存中的类型和地址来识别），它将返回已存在的Python对象封装，而不是创建一份拷贝。
2. 下一节将讨论上面表格之外的调用策略，他涉及到返回值和函数参数的引用关系。
3. 可以考虑使用智能指针来代替复杂的调用策略和生命周期管理逻辑。智能指针会告诉你一个对象是否仍被C++或Python引用，这样就可以消除各种可能引发crash或未定义行为的矛盾。对于返回智能指针的函数，没必要指定返回值策略。

