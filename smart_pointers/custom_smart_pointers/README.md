### 自定义智能指针

pybind11支持开箱即用的 std::unique_ptr 和 std::shared_ptr 。
对于其他自定义的智能指针，可以使用下面的宏使能透明转换（transparent conversions）。
它必须在其他绑定代码之前在顶层名称空间中声明：

```cpp
PYBIND11_DECLARE_HOLDER_TYPE(T, SmartPtr<T>);
```

宏的第一个参数为占位符名称，用作第二个参数的模板参数。
因此，你可以使用任意的标识符（不要使用你的代码中已经存在的类型），只需保持两边一致即可。

宏也可以接收第三个可选的bool类型参数，默认为false。

```cpp
PYBIND11_DECLARE_HOLDER_TYPE(T, SmartPtr<T>, true);
```

如果`SmartPtr<T>`总是从`T*`指针初始化，不存在不一致的风险（如多个独立的`SmartPtr<T>`认为他们是`T*`指针的唯一拥有者）。当T实例使用侵入式引用计数时，应设定为true。


默认情况下，pybind11假定自定义智能指针具有标准接口，如提供.get()成员函数来获取底层的原始指针。
如果没有，则需要指定holder_helper：

```cpp
// Always needed for custom holder types
PYBIND11_DECLARE_HOLDER_TYPE(T, SmartPtr<T>);

// Only needed if the type's `.get()` goes by another name
namespace pybind11 { namespace detail {
    template <typename T>
    struct holder_helper<SmartPtr<T>> { // <-- specialization
        static const T *get(const SmartPtr<T> &p) { return p.getPointer(); }
    };
}}
```

上述特化告诉pybind11，自定义SmartPtr通过`.getPointer()`提供`.get()`接口。

