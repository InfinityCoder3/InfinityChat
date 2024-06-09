#pragma once

#include <memory>
#include <mutex>
#include <iostream>

/// <summary>
/// 单例模板类
/// 不提供拷贝构造于拷贝运算符
/// </summary>
/// <typeparam name="T"></typeparam>
template <typename T>
class Singleton {
protected:
	Singleton() = default;
	Singleton(const Singleton<T>& st) = delete;
	Singleton& operator=(const Singleton<T>& st) = delete;
	static std::shared_ptr<T> _instance;
public:
	// 使用once_flag确保第一次实例化时的线程安全
	static std::shared_ptr<T> GetInstance() {
		// static类型的once_flag能够保证仅在第一次时完成初始化
		static std::once_flag s_flag;
		// call_once能够保证，new T这个操作完成后在返回。确保不会出现双重锁那样时序错误的问题
		/*
		 * 双重锁时存在一个问题，即：A * a = new A() 这个操作是由三部分组成的
		 * 1 调用allocate开辟内存
		 * 2 调用construct执行A的构造函数
		 * 3 调用赋值操作将地址赋值给a
		 * 步骤1是确定的，最先执行，然而步骤2和3却不是固定的顺序，有可能3在2前面，这时就会存在类的构造函数还没执行，却提前返回的类的地址
		*/
		std::call_once(s_flag, [&]() {
			// 调用T的构造函数，需要在子类T中，将Singleton<T>设为T的友元类
			// 并且，这里只能使用shared_prt，而不能使用make_shared<>
			_instance = std::shared_ptr<T>(new T);
			});
		return _instance;
	}

	void PrintAddress() {
		std::cout << "Singleton object address: " << _instance.get() << std::endl;
	}

	~Singleton() {
		std::cout << "This is singleton destruct.\n";
	}

};

// 静态成员变量需要类外初始化
template<typename T>
std::shared_ptr<T> Singleton<T>::_instance = nullptr;