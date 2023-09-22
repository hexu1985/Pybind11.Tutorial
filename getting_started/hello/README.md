### 第一个例子

在Linux上，你需要安装python3-dev包和cmake。

你需要git clone下来pybind11项目。

在CMakeLists.txt中，通过下面两句

```
add_subdirectory(pybind11)
pybind11_add_module(hello hello.cpp)
```

来构建python的C++扩展模块，编译出来的就是一个.so，且符合python模块规范的。

然后我们回过头来看一下C++扩展代码：

```cpp
#include <pybind11/pybind11.h>

namespace py = pybind11;

char const* greet()     // greet函数定义
{
   return "hello, world";
}

PYBIND11_MODULE(hello, m)  // 指示要扩展的python模块名为"hello"
{
    m.doc() = "pybind11 example hello";

    m.def("greet", greet, "A function which print \"hello, world\"");        // 将greet函数导出到python, 并且在python中函数名也叫"greet"
}
```

`PYBIND11_MODULE`会创建一个函数，它在Python中使用`import`语句时被调用。
宏的第一个参数是模块名（hello），不使用引号包住；第二个参数是类型为`py::module_`的变量（m），
它是创建绑定的主要接口。`module_::def()`方法，则会生成greet函数的Python绑定代码。

编译上面的C++代码后，我们会得到一个二进制模块文件，直接使用`import`导入模块到Python中。
假设编译好的模块位于当前目录下，Python交互示例代码如下：
