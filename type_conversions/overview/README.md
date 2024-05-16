### 概述

除了支持跨语言函数调用，pybind11这类绑定工具必须处理的一个基本问题就是，提供在C++中访问原生Python类型的方式，反之亦然。
有三种完全不同的方法做到这点，每种方法适用性取决于你使用的环境。

1. 任意侧使用原生的C++类型。这种情况下，必须使用pybind11生成类型的绑定，Python才能使用它。

2. 任意侧使用原生的Python类型。同样需要包装后，C++函数才能够使用它。

3. C++侧使用原生C++类型，Python侧使用原生Python类型。pybind11称其为类型转换。
   某种意义下，在任意侧使用原生类型，类型转换是最自然的选项。
   该方法主要的缺点是，每次Python和C++之间转换时都需要拷贝一份数据，因为C++和Python的对相同类型的内存布局不一样。 
   pybind11可以自动完成多种类型的转换。后面会提供所有内置转换的表格。

下面的小节将更详细地讨论这些选项之间的区别。


**1. Native type in C++, wrapper in Python**

在“面对对象编程”一章中，我们详细介绍了通过py::class_公开自定义C++类型的方法。
这里，底层的数据结构仍然是原始的C++类，而py::class_包装则提供了Python侧的接口。
当一个对象从C++侧发送到Python侧时，pybind11仅仅在原始的C++对象上增加了一层包装而已。
从Python侧获取它也仅仅是撕掉了包装而已。


**2. Wrapper in C++, native type in Python**

这与上面是完全相反的情况。现在我们有一个原生的Python类型，如tuple或list。在C++侧获取这个数据的一种方法是使用py::object族包装器。
这将在后续章节详细解释。这里举个简单的例子：

```cpp
void print_list(py::list my_list) {
    for (auto item : my_list)
        std::cout << item << " ";
}
>>> print_list([1, 2, 3])
1 2 3
```

Python的list仅仅是包裹在了C++ py::list类里，并没有仅仅任何转换。
它的核心任然是一个Python对象。拷贝一个py::list会像Python中一样增加引用计数。
将对象返回到Python侧，将去掉这层封装。


**3. Converting between native C++ and Python types**

前面两种情况，我们在一种语言中使用原生类型，而在另一种语言中使用它的包装类型。
现在，我们在两侧都使用原生类型，并对他们进行类型转换。

```cpp
void print_vector(const std::vector<int> &v) {
    for (auto item : v)
        std::cout << item << "\n";
}
>>> print_vector([1, 2, 3])
1 2 3
```

这个例子中，pybind11将创建一个`std::vector<int>`实例，并从Python list中拷贝每个元素。然后将该实例传递给print_vector。
同样的事情发生在另一个方向：新建了一个list，并从C++的vector中获取元素值。

如下表所示，多数转换是开箱即用的。他们相当方便，但请记住一点，这些转换是基于数据拷贝的。
这对小型的不变的类型相当友好，对于大型数据结构则相当昂贵。这可以通过自定义包装类型重载自动转换来规避（如上面提到的方法1）。


**内置转换的列表**

下面基础数据类型是开箱即用的（有些可能需要include额外的头文件）。

| Data type                                       | Description                                                  | Header file             |
| ----------------------------------------------- | ------------------------------------------------------------ | ----------------------- |
| `int8_t`, `uint8_t`                             | 8-bit integers                                               | `pybind11/pybind11.h`   |
| `int16_t`, `uint16_t`                           | 16-bit integers                                              | `pybind11/pybind11.h`   |
| `int32_t`, `uint32_t`                           | 32-bit integers                                              | `pybind11/pybind11.h`   |
| `int64_t`, `uint64_t`                           | 64-bit integers                                              | `pybind11/pybind11.h`   |
| `ssize_t`, `size_t`                             | Platform-dependent size                                      | `pybind11/pybind11.h`   |
| `float`, `double`                               | Floating point types                                         | `pybind11/pybind11.h`   |
| `bool`                                          | Two-state Boolean type                                       | `pybind11/pybind11.h`   |
| `char`                                          | Character literal                                            | `pybind11/pybind11.h`   |
| `char16_t`                                      | UTF-16 character literal                                     | `pybind11/pybind11.h`   |
| `char32_t`                                      | UTF-32 character literal                                     | `pybind11/pybind11.h`   |
| `wchar_t`                                       | Wide character literal                                       | `pybind11/pybind11.h`   |
| `const char *`                                  | UTF-8 string literal                                         | `pybind11/pybind11.h`   |
| `const char16_t *`                              | UTF-16 string literal                                        | `pybind11/pybind11.h`   |
| `const char32_t *`                              | UTF-32 string literal                                        | `pybind11/pybind11.h`   |
| `const wchar_t *`                               | Wide string literal                                          | `pybind11/pybind11.h`   |
| `std::string`                                   | STL dynamic UTF-8 string                                     | `pybind11/pybind11.h`   |
| `std::u16string`                                | STL dynamic UTF-16 string                                    | `pybind11/pybind11.h`   |
| `std::u32string`                                | STL dynamic UTF-32 string                                    | `pybind11/pybind11.h`   |
| `std::wstring`                                  | STL dynamic wide string                                      | `pybind11/pybind11.h`   |
| `std::string_view`, `std::u16string_view`, etc. | STL C++17 string views                                       | `pybind11/pybind11.h`   |
| `std::pair<T1, T2>`                             | Pair of two custom types                                     | `pybind11/pybind11.h`   |
| `std::tuple<...>`                               | Arbitrary tuple of types                                     | `pybind11/pybind11.h`   |
| `std::reference_wrapper<...>`                   | Reference type wrapper                                       | `pybind11/pybind11.h`   |
| `std::complex<T>`                               | Complex numbers                                              | `pybind11/complex.h`    |
| `std::array<T, Size>`                           | STL static array                                             | `pybind11/stl.h`        |
| `std::vector<T>`                                | STL dynamic array                                            | `pybind11/stl.h`        |
| `std::deque<T>`                                 | STL double-ended queue                                       | `pybind11/stl.h`        |
| `std::valarray<T>`                              | STL value array                                              | `pybind11/stl.h`        |
| `std::list<T>`                                  | STL linked list                                              | `pybind11/stl.h`        |
| `std::map<T1, T2>`                              | STL ordered map                                              | `pybind11/stl.h`        |
| `std::unordered_map<T1, T2>`                    | STL unordered map                                            | `pybind11/stl.h`        |
| `std::set<T>`                                   | STL ordered set                                              | `pybind11/stl.h`        |
| `std::unordered_set<T>`                         | STL unordered set                                            | `pybind11/stl.h`        |
| `std::optional<T>`                              | STL optional type (C++17)                                    | `pybind11/stl.h`        |
| `std::experimental::optional<T>`                | STL optional type (exp.)                                     | `pybind11/stl.h`        |
| `std::variant<...>`                             | Type-safe union (C++17)                                      | `pybind11/stl.h`        |
| `std::filesystem::path<T>`                      | STL path (C++17) [1](https://pybind11.readthedocs.io/en/stable/advanced/cast/overview.html#id2) | `pybind11/stl.h`        |
| `std::function<...>`                            | STL polymorphic function                                     | `pybind11/functional.h` |
| `std::chrono::duration<...>`                    | STL time duration                                            | `pybind11/chrono.h`     |
| `std::chrono::time_point<...>`                  | STL date/time                                                | `pybind11/chrono.h`     |
| `Eigen::Matrix<...>`                            | Eigen: dense matrix                                          | `pybind11/eigen.h`      |
| `Eigen::Map<...>`                               | Eigen: mapped memory                                         | `pybind11/eigen.h`      |
| `Eigen::SparseMatrix<...>`                      | Eigen: sparse matrix                                         | `pybind11/eigen.h`      |


