# 1.设计总览

- 我们首先创建一个GateServer. cpp文件，绑定好端口后，便调用调用io_context服务，使得整个进程循环的跑起来，如果没有往io_context服务中注册事件，程序便会退出，如果注册了，则会一直运行下去
- 往io_context注册监听服务，需要CServer服务器类，提供一个Start方法，该方法的作用是使用async_accept函数异步监听链接事件。
- 当有新的链接过来时，我们便调用新连接对应的处理函数Start。处理连接的类叫做HttpConnection。在Start函数中，首先往ioc的这个socket中，注册一个异步读事件async_read，因为作为服务器来说，一般都是被动的一方。
- 当读事件被调用后，则对读请求进行处理，这里需要区分Get请求和Post请求，http还有其他请求，这里仅先对这两种进行判断解析。
- 为了使得程序结构更为清晰，独立出一个逻辑类，专门处理收到的http请求，名为LogicSystem，逻辑处理层，把解析成功与否的结果返回给HttpConnection层中即可，然后再在HttpConnection层，做进一步的解析，并将处理结果返回客户端
- 由于http并不需要保持长连接状态，所以设置了一个心跳机制，每个60秒检测一次，如果期间没有数据交互，则将该链接关闭。





#  2. 使用 `std::make_shared<T>` 和 `std::shared_ptr<T>(new T)` 的区别

**区别**：

- `std::make_shared<T>`：这个函数是 C++11 中引入的，用于创建一个 `std::shared_ptr` 对象。它一次性分配内存，用于存储对象和控制块（引用计数）。这种方法更高效，且异常安全，因为它确保在分配内存时发生异常时不会导致内存泄漏。

- `std::shared_ptr<T>(new T)`：这种方法首先分配对象的内存，然后创建一个独立的控制块。由于分配对象和控制块的操作是分开的，所以在异常安全性和内存使用上不如 `std::make_shared`。

**推荐使用**：

`std::make_shared<T>` 通常是更好的选择，因为它更高效且异常安全。

**示例**：

```cpp
// 推荐的方式
_instance = std::make_shared<T>();

// 不推荐的方式
_instance = std::shared_ptr<T>(new T);
```

**总结**：

- `std::make_shared<T>` 更高效，分配一次内存，适合大多数情况。
- `std::shared_ptr<T>(new T)` 分配两次内存，适合需要自定义删除器或某些特定情况。

# 3. 在 `MyClass` 中不声明 `friend class Singleton<MyClass>;` 是否可行

**问题分析**：

`friend class Singleton<MyClass>;` 声明允许 `Singleton<MyClass>` 访问 `MyClass` 的私有构造函数。如果不声明友元类，那么 `Singleton<MyClass>` 无法访问 `MyClass` 的私有构造函数，无法创建其实例。

**示例**：

如果不声明友元类，代码将无法编译：

```cpp
class MyClass : public Singleton<MyClass> {
private:
    MyClass() {
        std::cout << "MyClass instance created.\n";
    }
};

int main() {
    auto instance1 = MyClass::GetInstance();  // 编译错误
    instance1->PrintAddress();

    return 0;
}
```

编译时将会出现类似以下的错误：

```
error: ‘MyClass::MyClass()’ is private within this context
```

**解决方案**：

确保 `Singleton<MyClass>` 能够访问 `MyClass` 的私有构造函数，通过友元声明实现：

```cpp
class MyClass : public Singleton<MyClass> {
    friend class Singleton<MyClass>;  // 允许 Singleton<MyClass> 访问私有构造函数
private:
    MyClass() {
        std::cout << "MyClass instance created.\n";
    }
};
```

**总结**：

- 需要 `friend class Singleton<MyClass>;` 声明来允许 `Singleton<MyClass>` 访问 `MyClass` 的私有构造函数，否则无法实例化对象。
- 友元声明确保 `Singleton` 模板类能正确创建子类的单例实例。

### 最终代码

结合上述解释，最终的代码应如下：

```cpp
#include <iostream>
#include <memory>
#include <mutex>

// Singleton 模板类定义
template <typename T>
class Singleton {
protected:
    Singleton() = default;
    Singleton(const Singleton<T>&) = delete;
    Singleton& operator=(const Singleton<T>&) = delete;

    static std::shared_ptr<T> _instance;

public:
    static std::shared_ptr<T> GetInstance() {
        static std::once_flag s_flag;
        std::call_once(s_flag, []() {
            // 使用 make_shared 创建实例
            _instance = std::make_shared<T>();
        });
        return _instance;
    }

    void PrintAddress() {
        std::cout << _instance.get() << std::endl;
    }

    ~Singleton() {
        std::cout << "This is singleton destruct.\n";
    }
};

template <typename T>
std::shared_ptr<T> Singleton<T>::_instance = nullptr;

// 示例子类
class MyClass : public Singleton<MyClass> {
    friend class Singleton<MyClass>;  // 允许 Singleton<MyClass> 访问私有构造函数
private:
    MyClass() {
        std::cout << "MyClass instance created.\n";
    }
};

int main() {
    auto instance1 = MyClass::GetInstance();
    instance1->PrintAddress();

    auto instance2 = MyClass::GetInstance();
    instance2->PrintAddress();

    return 0;
}
```

这样，代码既高效、安全，又能正确地实现单例模式。