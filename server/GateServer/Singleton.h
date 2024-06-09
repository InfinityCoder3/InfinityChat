#pragma once

#include <memory>
#include <mutex>
#include <iostream>

/// <summary>
/// ����ģ����
/// ���ṩ���������ڿ��������
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
	// ʹ��once_flagȷ����һ��ʵ����ʱ���̰߳�ȫ
	static std::shared_ptr<T> GetInstance() {
		// static���͵�once_flag�ܹ���֤���ڵ�һ��ʱ��ɳ�ʼ��
		static std::once_flag s_flag;
		// call_once�ܹ���֤��new T���������ɺ��ڷ��ء�ȷ���������˫��������ʱ����������
		/*
		 * ˫����ʱ����һ�����⣬����A * a = new A() �������������������ɵ�
		 * 1 ����allocate�����ڴ�
		 * 2 ����constructִ��A�Ĺ��캯��
		 * 3 ���ø�ֵ��������ַ��ֵ��a
		 * ����1��ȷ���ģ�����ִ�У�Ȼ������2��3ȴ���ǹ̶���˳���п���3��2ǰ�棬��ʱ�ͻ������Ĺ��캯����ûִ�У�ȴ��ǰ���ص���ĵ�ַ
		*/
		std::call_once(s_flag, [&]() {
			// ����T�Ĺ��캯������Ҫ������T�У���Singleton<T>��ΪT����Ԫ��
			// ���ң�����ֻ��ʹ��shared_prt��������ʹ��make_shared<>
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

// ��̬��Ա������Ҫ�����ʼ��
template<typename T>
std::shared_ptr<T> Singleton<T>::_instance = nullptr;