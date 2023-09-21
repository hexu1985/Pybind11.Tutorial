## 指定扩展模块的python版本

我们以python3.8为例，需要你已经安装了python3.8-dev包。

在CMakeLists.txt中，通过下面两句

```
set(PYTHON_TARGET_VER 3.8)
find_package(PythonInterp ${PYTHON_TARGET_VER} EXACT REQUIRED)
```

来指定扩展模块的python版本，其他的不变。

