### Exporting variables

To expose a value from C++, use the attr function to register it in a module as shown below. Built-in types and general objects (more on that later) are automatically converted when assigned as attributes, and can be explicitly converted using the function py::cast.

```cpp
PYBIND11_MODULE(example, m) {
    m.attr("the_answer") = 42;
    py::object world = py::cast("World");
    m.attr("what") = world;
}
```

These are then accessible from Python:

```python
>>> import example
>>> example.the_answer
42
>>> example.what
'World'
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
>>> example.the_answer
42
>>> example.what
'World'
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


