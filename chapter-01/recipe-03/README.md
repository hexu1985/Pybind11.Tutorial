### Keyword arguments

With a simple code modification, it is possible to inform Python about the names of the arguments (“i” and “j” in this case).

```cpp
m.def("add", &add, "A function which adds two numbers",
      py::arg("i"), py::arg("j"));
```

arg is one of several special tag classes which can be used to pass metadata into module::def(). With this modified binding code, we can now call the function using keyword arguments, which is a more readable alternative particularly for functions taking many parameters:

```python
>>> import example
>>> example.add(i=1, j=2)
3L
```

The keyword names also appear in the function signatures within the documentation.

```python
>>> help(example)

....

FUNCTIONS
    add(...)
        Signature : (i: int, j: int) -> int

        A function which adds two numbers
```

A shorter notation for named arguments is also available:

```
// regular notation
m.def("add1", &add, py::arg("i"), py::arg("j"));
// shorthand
using namespace pybind11::literals;
m.def("add2", &add, "i"_a, "j"_a);
```

The `_a` suffix forms a C++11 literal which is equivalent to arg. Note that the literal operator must first be made visible with the directive using namespace pybind11::literals. This does not bring in anything else from the pybind11 namespace except for literals.

#### 编译并测试程序

1. 编译example.so：如果想要生成python2的module, 需要修改Makefile中的INCLUDES

```shell
$ make
```

2. 调用example

```python
$ python
>>> import example
>>> example.add(i=1, j=2)
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

