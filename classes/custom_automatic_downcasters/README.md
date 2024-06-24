### 定制自动向下转型

pybind11内置了对C++多态的动态类型的处理。
有时，你可能希望提供的自动向下转型的行为 不使用标准 C++ 多态性（如 LLVM）。
只要有某种方法可以在运行时确定 向下转型 是否安全，就可以通过专门化 pybind11::polymorphic_type_hook 模板来完成：

```cpp
enum class PetKind { Cat, Dog, Zebra };
struct Pet {   // Not polymorphic: has no virtual methods
    const PetKind kind;
    int age = 0;
  protected:
    Pet(PetKind _kind) : kind(_kind) {}
};
struct Dog : Pet {
    Dog() : Pet(PetKind::Dog) {}
    std::string sound = "woof!";
    std::string bark() const { return sound; }
};

namespace PYBIND11_NAMESPACE {
    template<> struct polymorphic_type_hook<Pet> {
        static const void *get(const Pet *src, const std::type_info*& type) {
            // note that src may be nullptr
            if (src && src->kind == PetKind::Dog) {
                type = &typeid(Dog);
                return static_cast<const Dog*>(src);
            }
            return src;
        }
    };
} // namespace PYBIND11_NAMESPACE
```

当 pybind11 想要将 Base* 类型的C++指针转换为 Python 对象时，它会调用 `polymorphic_type_hook<Base>::get()` 来确定是否可以进行 向下转型。
get() 函数应该使用任何可用的运行时信息来确定其 src 参数是否实际上是从 Base 继承的某个类 Derived 的实例。
如果找到这样的 Derived，则设置 `type = &typeid(Derived)` 并返回指向包含 src 的 Derived 对象的指针。
否则，它只返回 src，将 type 保留为其默认值 nullptr。
如果将 type 设置为 pybind11 不知道的类型，则不会发生 向下转型，并且原始 src 指针将与其静态类型 Base* 一起使用。

get() 返回的指针和 type 参数必须相互一致：如果 type 设置为非空，则返回的指针必须指向类型为 type 的对象的开头。
如果公开的层次结构仅使用单个继承，则简单返回 src；可以很好地实现这一点，但在一般情况下，您必须将 src 强制转换为适当的派生类指针
（例如使用 `static_cast<Derived*>(src)`），然后才能允许它作为 void* 返回。

