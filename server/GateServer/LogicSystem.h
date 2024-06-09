#pragma once
#include "const.h"

class HttpConnection;
// http���������
typedef std::function<void(std::shared_ptr<HttpConnection>)> HttpHandler;

class LogicSystem : public Singleton<LogicSystem>
{
	/*
	* ��Singleton<LogicSystem>����Ϊ��Ԫ������Singleton<LogicSystem>����LogicSystem��˽�й��캯��
	*/


public:
	friend class Singleton<LogicSystem>;
	~LogicSystem();
	// ����Get����
	bool HandleGet(std::string path, std::shared_ptr<HttpConnection> con);
	// ����Post����
	bool HandlePost(std::string path, std::shared_ptr<HttpConnection> con);
	// ��map��ע��Get����
	void RegGet(std::string url, HttpHandler handler);
	// ��map��ע��Post����
	void RegPost(std::string url, HttpHandler handler);

private:
	LogicSystem();

private:
	std::unordered_map<std::string, HttpHandler> _post_handlers;
	std::unordered_map<std::string, HttpHandler> _get_handlers;


};

