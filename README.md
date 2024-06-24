# pybind11 Tutorial

## Table of contents


### [pybind11入门](getting_started)
- [第一个例子](getting_started/hello)
- [指定扩展模块的python版本](getting_started/specify_python_version)


### [基础用法](basic_features)
- [绑定函数](basic_features/binding_functions)
- [关键字参数](basic_features/keyword_arguments)
- [参数默认值](basic_features/default_arguments)
- [重载函数](basic_features/overloaded_functions)
- [导出变量](basic_features/exporting_variables)


### [面向对象编程](object_oriented_code)
- [创建一个自定义类的绑定](object_oriented_code/binding_custom_types)
- [绑定匿名函数](object_oriented_code/binding_lambda_functions)
- [成员变量](object_oriented_code/instance_and_static_fields)
- [动态属性](object_oriented_code/dynamic_attributes)
- [继承与向下转型](object_oriented_code/inheritance_and_automatic_downcasting)
- [重载方法](object_oriented_code/overloaded_methods)
- [枚举和内部类型](object_oriented_code/enumarations_and_internal_types)


### [函数](functions)
- [返回值策略](functions/return_value_policies)
- [附加的调用策略](functions/additional_call_policies)
- [以Python对象作为参数](functions/python_objects_as_arguments)
- [接收`*args`和`**kwatgs`参数](functions/accepting_args_and_kwargs)
- [再探默认参数](functions/default_arguments_revisited)
- [Keyword-only参数](functions/keyword_only_arguments)
- [Positional-only参数](functions/positional_only_arguments)
- [Non-converting参数](functions/non_converting_arguments)
- [允许/禁止空参数](functions/allow_prohibiting_none_arguments)
- [重载解析顺序](functions/overload_resolution_order)


### [类](classes)
- [在Python中重载虚函数](classes/overriding_virtual_functions)
- [虚函数与继承](classes/combining_virtual_functions_and_inheritance)
- [扩展跳板类的功能](classes/extended_trampoline_class_functionality)
- [定制构造函数](classes/custom_constructors)
- [非公有析构函数](classes/non_public_destructors)
- [在析构函数中调用Python](classes/destructors_that_call_python)
- [隐式转换](classes/implicit_conversions)
- [静态属性](classes/static_properties)
- [重载操作符](classes/operator_overloading)
- [支持pickle](classes/pickling_support)
- [深拷贝支持](classes/deepcopy_support)
- [多重继承](classes/multiple_inheritance)
- [绑定Module-local类](classes/module_local_class_bindings)
- [绑定protected成员函数](classes/binding_protected_member_functions)
- [绑定final类](classes/binding_final_classes)
- [绑定模板类](classes/binding_classes_with_template_parameters)
- [定制自动向下转型](classes/custom_automatic_downcasters)
- [访问类型对象](classes/accessing_the_type_object)


### [异常](exceptions)
- [C++内置异常到Python异常的转换](exceptions/build_in_exception_translation)
- [注册自定义异常翻译](exceptions/registering_custom_translators)
- [在C++中处理Python异常](exceptions/handling_python_excepitons_in_cxx)
- [处理Python C API的错误](exceptions/handling_python_c_api_errors)
- [异常链（raise from）](exceptions/chaining_exceptions)
- [处理unraiseable异常](exceptions/handling_unraisable_exceptions)


### [智能指针](smart_pointers)
- [std::unique_ptr](smart_pointers/unique_ptr)
- [std::shared_ptr](smart_pointers/shared_ptr)
- [自定义智能指针](smart_pointers/custom_smart_pointers)


### [类型转换](type_conversions)
- [概述](type_conversions/overview)
- [Strings、bytes和Unicode转换](strings_bytes_and_unicode_conversions)
- [STL容器](type_conversions/stl_containers)


### 参考链接

- pybind11官网： <https://pybind11.readthedocs.io/en/latest/index.html>
- pybind11中文文档：<https://github.com/charlotteLive/pybind11-Chinese-docs>  <https://charlottelive.github.io/pybind11-Chinese-docs/>
- github项目：<https://github.com/pybind/pybind11>
