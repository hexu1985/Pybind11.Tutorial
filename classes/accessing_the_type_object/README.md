### 访问类型对象

我们可以从已注册的C++类，获取到类型对象：

```cpp
py::type T_py = py::type::of<T>();
```

也可以直接使用py::type::of(ob)来获取任意Python对象的类型，跟Python中的type(ob)一样。


