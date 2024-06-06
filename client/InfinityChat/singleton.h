#ifndef SINGLETON_H
#define SINGLETON_H

#include "global.h"

template <typename T>
class Singleton {
    //不允许外部对单例类机型构造，但是可以让子类去继承，所以这里使用protected
protected:
    Singleton() = default;
    Singleton(const Singleton<T>& ) = delete;
    Singleton& operator=(const Singleton<T>& ) = delete;
    //静态成员变量，所有使用该类的对象都返回同一个实例
    static std::shared_ptr<T> _instance;
public:
    static std::shared_ptr<T> GetInstance() {
        // once_flag只会被初始化一次
        static std::once_flag s_falg;
        //call_once 只有当s_falg为true时才会执行后面的函数
        std::call_once(s_falg, [&]() {
            // 继承模版单例类的子类的构造会设置为private，make_shared无法访问私有的构造函数
            //为什么不适用make_shared<T>(new T);？
//            _instance = std::make_shared<T>();
            /*
             * std::make_shared<T> 无法访问私有构造函数的原因是它在自己的上下文中调用构造函数，不受 friend 声明的影响。
             * std::shared_ptr<T>(new T) 可以在友元或成员函数中直接调用，因为此时上下文允许访问私有构造函数
             * 即make_shared<T>是在其实现内部调用T的构造函数，不是在Singleton<T>的上下文中调用的。
             * 因此无法构造，子类中的友元类声明是针对Singleton<T>的上下文。
            */
            _instance = std::shared_ptr<T>(new T);
        });
        return _instance;
    }

    void PrintAddress() {
        std::cout << _instance.get() << std::endl;
    }
    ~Singleton() {
        std::cout << "this is singleton destruct.\n";
    }
private:

};

template <typename T>
std::shared_ptr<T> Singleton<T>::_instance = nullptr;


#endif // SINGLETON_H
