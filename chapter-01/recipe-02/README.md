### Creating bindings for a simple function

For brevity, all code examples assume that the following two lines are present:

```cpp
#include <pybind11/pybind11.h>

namespace py = pybind11;
```

Some features may require additional headers, but those will be specified as needed.

Let’s start by creating Python bindings for an extremely simple function, which adds two numbers and returns their result:

```cpp
int add(int i, int j) {
    return i + j;
}
```

For simplicity, we’ll put both this function and the binding code into a file named example.cpp with the following contents:

```cpp
#include <pybind11/pybind11.h>

int add(int i, int j) {
    return i + j;
}

PYBIND11_MODULE(example, m) {
    m.doc() = "pybind11 example plugin"; // optional module docstring

    m.def("add", &add, "A function which adds two numbers");
}
```

The PYBIND11_MODULE() macro creates a function that will be called when an import statement is issued from within Python. The module name (example) is given as the first macro argument (it should not be in quotes). The second argument (m) defines a variable of type py::module which is the main interface for creating bindings. The method module::def() generates binding code that exposes the add() function to Python.

pybind11 is a header-only library, hence it is not necessary to link against any special libraries and there are no intermediate (magic) translation steps. On Linux, the above example can be compiled using the following command:

```shell
$ c++ -O3 -Wall -shared -std=c++11 -fPIC `python3 -m pybind11 --includes` example.cpp -o example`python3-config --extension-suffix`
```

Building the above C++ code will produce a binary module file that can be imported to Python. Assuming that the compiled module is located in the current directory, the following interactive Python session shows how to load and execute the example:

```python
$ python
>>> import example
>>> example.add(1, 2)
3L
>>>
```

#### 编译并测试程序

1. 编译example.so：如果想要生成python2的module, 需要修改Makefile中的INCLUDES

```shell
$ make
```

2. 调用example

```python
$ python
>>> import example
>>> example.add(1, 2)
3L
```

3. 通过脚本调用example

```shell
$ python test_example.py
```

4. 通过CMake编译程序

```shell
mkdir build
cmake -H. -Bbuild -Dpybind11_DIR=~/local/share/cmake/pybind11
cmake --build build
```


