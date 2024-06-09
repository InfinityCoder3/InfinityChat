#pragma once
#include "const.h"

class HttpConnection;
// http请求处理对象
typedef std::function<void(std::shared_ptr<HttpConnection>)> HttpHandler;

class LogicSystem : public Singleton<LogicSystem>
{
	/*
	* 将Singleton<LogicSystem>设置为友元类允许Singleton<LogicSystem>访问LogicSystem的私有构造函数
	*/


public:
	friend class Singleton<LogicSystem>;
	~LogicSystem();
	// 调用Get请求
	bool HandleGet(std::string path, std::shared_ptr<HttpConnection> con);
	// 调用Post请求
	bool HandlePost(std::string path, std::shared_ptr<HttpConnection> con);
	// 往map中注册Get请求
	void RegGet(std::string url, HttpHandler handler);
	// 往map中注册Post请求
	void RegPost(std::string url, HttpHandler handler);

private:
	LogicSystem();

private:
	std::unordered_map<std::string, HttpHandler> _post_handlers;
	std::unordered_map<std::string, HttpHandler> _get_handlers;


};

