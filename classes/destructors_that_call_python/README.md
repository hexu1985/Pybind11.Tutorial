### 在析构函数中调用Python

在析构函数中调用Python函数出错时，会抛出异常error_already_set。
如果异常在析构函数外抛出，将会调用std::terminate()来终结程序。
因此，类析构函数必须捕获所有error_already_set类型的异常，并使用error_already_set::discard_as_unraisable()来丢弃Python异常。

任意Python函数都可能抛出异常。
比如一个Python生成器停止生成条目时，Pyhton将抛出StopIteration异常，如果生成器的堆栈持有C++对象的最后一个引用时，它将传递异常到C++析构函数。


```cpp
class MyClass {
public:
    ~MyClass() {
        try {
            py::print("Even printing is dangerous in a destructor");
            py::exec("raise ValueError('This is an unraisable exception')");
        } catch (py::error_already_set &e) {
            // error_context should be information about where/why the occurred,
            // e.g. use __func__ to get the name of the current function
            e.discard_as_unraisable(__func__);
        }
    }
};
```
