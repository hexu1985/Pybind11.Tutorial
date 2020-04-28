### Default arguments

Suppose now that the function to be bound has default arguments, e.g.:

```cpp
int add(int i = 1, int j = 2) {
    return i + j;
}
```

Unfortunately, pybind11 cannot automatically extract these parameters, since they are not part of the function’s type information. However, they are simple to specify using an extension of arg:

```cpp
m.def("add", &add, "A function which adds two numbers",
      py::arg("i") = 1, py::arg("j") = 2);
```

The default values also appear within the documentation.

```python
>>> help(example)

....

FUNCTIONS
    add(...)
        Signature : (i: int = 1, j: int = 2) -> int

        A function which adds two numbers
```

The shorthand notation is also available for default arguments:

```cpp
// regular notation
m.def("add1", &add, py::arg("i") = 1, py::arg("j") = 2);
// shorthand
m.def("add2", &add, "i"_a=1, "j"_a=2);
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
>>> example.add()
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

5. 使用`A shorter notation for named arguments`，需要修改Makefile的CXXFLAGS，增加`-DUSE_PYBIND11_LITERALS`，
如果是通过cmake编译项目的化，需要cmake的时候指定`CMAKE_CXX_FLAGS`：`-DCMAKE_CXX_FLAGS="-DUSE_PYBIND11_LITERALS"`

