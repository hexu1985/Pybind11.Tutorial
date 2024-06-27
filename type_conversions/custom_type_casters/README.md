### 自定义类型转换

在极少数情况下，程序可能需要一些pybind11没有提供的自定义类型转换，这需要使用到原始的Python C API。
这是相当高级的使用方法，只有熟悉Python引用计数复杂之处的专家才能使用。

以下代码片段展示了如何适配于一个非常简单的 inty 类型，该类型应该可以从提供 __int__（self） 方法的 Python 类型转换而来。

```cpp
struct inty { long long_value; };

void print(inty s) {
    std::cout << s.long_value << std::endl;
}
```

以下 Python 代码片段展示了 Python 端的预期用法：

```python
class A:
    def __int__(self):
        return 123


from example import print

print(A())
```

要注册必要的转换例程，必须添加 `pybind11::detail::type_caster<T>` 模板。
尽管这是一个实现细节，但明确允许添加此类型的实例化。

```cpp
namespace pybind11 { namespace detail {
    template <> struct type_caster<inty> {
    public:
        /**
         * This macro establishes the name 'inty' in
         * function signatures and declares a local variable
         * 'value' of type inty
         */
        PYBIND11_TYPE_CASTER(inty, _("inty"));

        /**
         * Conversion part 1 (Python->C++): convert a PyObject into a inty
         * instance or return false upon failure. The second argument
         * indicates whether implicit conversions should be applied.
         */
        bool load(handle src, bool) {
            /* Extract PyObject from handle */
            PyObject *source = src.ptr();
            /* Try converting into a Python integer value */
            PyObject *tmp = PyNumber_Long(source);
            if (!tmp)
                return false;
            /* Now try to convert into a C++ int */
            value.long_value = PyLong_AsLong(tmp);
            Py_DECREF(tmp);
            /* Ensure return code was OK (to avoid out-of-range errors etc) */
            return !(value.long_value == -1 && !PyErr_Occurred());
        }

        /**
         * Conversion part 2 (C++ -> Python): convert an inty instance into
         * a Python object. The second and third arguments are used to
         * indicate the return value policy and parent object (for
         * ``return_value_policy::reference_internal``) and are generally
         * ignored by implicit casters.
         */
        static handle cast(inty src, return_value_policy /* policy */, handle /* parent */) {
            return PyLong_FromLong(src.long_value);
        }
    };
}} // namespace pybind11::detail
```

> Note: 用 PYBIND11_TYPE_CASTER(T, ...) 定义的 type_caster<T> 要求 T 有默认构造函数（value 首先默认构造，然后调用 load() 给它赋值）。

> Warning：使用自定义类型转换器时，请务必在 Python 扩展模块的每个编译单元中一致地声明它们。
> 否则，可能会出现未定义的行为。


