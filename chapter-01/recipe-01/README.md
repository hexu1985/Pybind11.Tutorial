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
