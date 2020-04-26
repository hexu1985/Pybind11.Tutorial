### Compiling the test cases

#### Linux/MacOS

On Linux you’ll need to install the python-dev or python3-dev packages as well as cmake. On Mac OS, the included python version works out of the box, but cmake must still be installed.

After installing the prerequisites, run

```shell
mkdir build
cd build
cmake ..
make check -j 4
```

The last line will both compile and run the tests.

#### Windows

On Windows, only Visual Studio 2015 and newer are supported since pybind11 relies on various C++11 language features that break older versions of Visual Studio.

To compile and run the tests:

```shell
mkdir build
cd build
cmake ..
cmake --build . --config Release --target check
```

This will create a Visual Studio project, compile and run the target, all from the command line.

#### pybind11安装到指定目录

安装pybind11项目需要依赖`cmake`：

```
$ git clone https://github.com/pybind/pybind11.git pybind11
$ cd pybind11
$ mkdir build
$ cd build
$ cmake -DCMAKE_INSTALL_PREFIX=~/local ..
$ cmake --build . --target install
```

或者更简单的方式

```
$ git clone https://github.com/pybind/pybind11.git pybind11
$ cd pybind11
$ cmake -H. -Bbuild -DCMAKE_INSTALL_PREFIX=~/local
$ cmake --build build --target install
```

CMAKE_INSTALL_PREFIX指定安装目录前缀，可能需要sudo权限执行install命令
