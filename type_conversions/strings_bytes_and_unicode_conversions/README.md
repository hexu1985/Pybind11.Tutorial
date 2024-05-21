### Strings, bytes and Unicode conversions

#### 传递Python strings到C++

当向一个接收`std::string`或`char *`参数的函数传递Python的str时，pybind11会将Python字符串编码为UTF-8。
所有的Python str都能够用UTF-8编码，所以这个操作不会失败。

C++语言是encoding agnostic。程序员负责处理编码，最简单的做法就是每处都使用UTF-8。

```cpp
m.def("utf8_test",
    [](const std::string &s) {
        cout << "utf-8 is icing on the cake.\n";
        cout << s;
    }
);
m.def("utf8_charptr",
    [](const char *s) {
        cout << "My favorite food is\n";
        cout << s;
    }
);
```

```python
>>> utf8_test("🎂")
utf-8 is icing on the cake.
🎂

>>> utf8_charptr("🍕")
My favorite food is
🍕
```

> Note: 有些终端模拟器不支持UTF-8或emoji字体，上面的例子可能无法显示。

无论C++函数的参数是传值或引用，是否是const，结果都是一样的。


**向C++传递bytes对象**

向接收`std::string`或`char *`类型参数的C++函数传递Python bytes对象无需转换。
在Python3上，如果想要函数只接收bytes，不接收str，可以声明参数类型为py::bytes。

---

#### 向Python返回C++ 字符串

当C++函数返回`std::string`或`char*`参数给Python调用者时，pybind11会将字符串以UTF-8格式解码给原生Python str，类似于Python中的bytes.decode('utf-8')。
如果隐式转换失败，pybind11将会抛出异常UnicodeDecodeError。

```cpp
m.def("std_string_return",
    []() {
        return std::string("This string needs to be UTF-8 encoded");
    }
);
```

```python
>>> isinstance(example.std_string_return(), str)
True
```

因为UTF-8包含纯ASCII，返回一个纯ASCII字符串到Python没有任何问题。否则就需要确保编码是有效的UTF-8。

> Warning: 隐式转换假定char *字符串以null为结束符。若不是，将导致缓冲区溢出。


**显式转换**

如果C++代码构造了一个非UTF-8的string字符串，可以执行显式转换并返回py::str对象。显式转换与隐式转换的开销相同。

```cpp
// This uses the Python C API to convert Latin-1 to Unicode
m.def("str_output",
    []() {
        std::string s = "Send your r\xe9sum\xe9 to Alice in HR"; // Latin-1
        py::handle py_s = PyUnicode_DecodeLatin1(s.data(), s.length(), nullptr);
        if (!py_s) {
            throw py::error_already_set();
        }
        return py::reinterpret_steal<py::str>(py_s);
    }
);
```

```python
>>> str_output()
'Send your résumé to Alice in HR'
```

Python C API提供了一些内置的编解码方法可以使用。也可以使用第三方库如libiconv 来转换UTF-8。


**不使用类型转换来返回C++字符串**

如果C++ std::string中的数据不表示文本，则应该以bytes的形式传递给Python，这时我们可以返回一个py::btyes对象。

```cpp
m.def("return_bytes",
    []() {
        std::string s("\xba\xd0\xba\xd0");  // Not valid UTF-8
        return py::bytes(s);  // Return the data without transcoding
    }
);
```

```python
>>> example.return_bytes()
b'\xba\xd0\xba\xd0'
```

注意：pybind11可以将bytes无需编码地转换为std::string，但不能不经编码地隐式转换std::string到bytes。

```cpp
m.def("asymmetry",
    [](std::string s) {  // Accepts str or bytes from Python
        return s;  // Looks harmless, but implicitly converts to str
    }
);
```

```python
>>> isinstance(example.asymmetry(b"have some bytes"), str)
True

>>> example.asymmetry(b"\xba\xd0\xba\xd0")  # invalid utf-8 as bytes
UnicodeDecodeError: 'utf-8' codec can't decode byte 0xba in position 0: invalid start byte
```

---

#### 宽字符串

向入参为`std::wstring`、`wchar_t*`、`std::u16string`或`std::u32string`的C++函数传递Python str对象，
str将被编码为UTF-16或UTF-32（具体哪种取决于C++编译器的支持）。
当C++函数返回这些类型的字符串到Python str时，需要保证字符串是合法的UTF-16或UTF-32。


```cpp
#define UNICODE
#include <windows.h>

m.def("set_window_text",
    [](HWND hwnd, std::wstring s) {
        // Call SetWindowText with null-terminated UTF-16 string
        ::SetWindowText(hwnd, s.c_str());
    }
);

m.def("get_window_text",
    [](HWND hwnd) {
        const int buffer_size = ::GetWindowTextLength(hwnd) + 1;
        auto buffer = std::make_unique< wchar_t[] >(buffer_size);

        ::GetWindowText(hwnd, buffer.data(), buffer_size);

        std::wstring text(buffer.get());

        // wstring will be converted to Python str
        return text;
    }
);
```

> 警告：带--enable-unicode=ucs2选项编译的Python 2.7和3.3版本可能不支持上述的宽字符串。

多字节编码地字符串，如Shift-JIS，必须转换为UTF-8/16/32后，再返回给Python。

---

#### 字符类型

向一个入参为字符类型（char, wchar_t）的C++函数，传递Python str，C++函数将接收str的首字符。
如果字符串超过一个Unicode字符长度，将忽略尾部字节。

当C++返回一个字符变量时，它将被转化为单字符的str变量。

```cpp
m.def("pass_char", [](char c) { return c; });
m.def("pass_wchar", [](wchar_t w) { return w; });
```

```python
>>> example.pass_char("A")
'A'
```

虽然C++可以将整数转换为字符类型（char c = 0x65），pybind11并不会隐式转换Python整数到字符类型。
可以使用chr()Python函数来将整数转换为字符。

```python
>>> example.pass_char(0x65)
TypeError

>>> example.pass_char(chr(0x65))
'A'
```

如果需要使用8-bit整数，可使用int8_t或uint8_t作为参数类型。
