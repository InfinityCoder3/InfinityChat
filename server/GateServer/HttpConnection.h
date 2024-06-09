#pragma once

#include "const.h"

class HttpConnection : public std::enable_shared_from_this<HttpConnection>
{
	friend class LogicSystem;
public:
	HttpConnection(net::io_context& ioc);
	void Start();

	tcp::socket& GetSocket() {
		return _socket;
	}
private:
	// 检测心跳
	void CheckDeadline();
	// http回消息
	void WriteResponce();
	// http处理请求消息
	void HandleReq();
	// 解析数据
	void PreParseGetParam();
private:
	tcp::socket _socket;	//用于通信的socket
	beast::flat_buffer _buffer{ 8192 };		//接收数据缓存
	http::request<http::dynamic_body> _request;		// request message
	http::response<http::dynamic_body> _responce;	// responce message
	// 底层实现需要去轮询 绑定底层的调度器
	net::steady_timer _deadline{ _socket.get_executor(), std::chrono::seconds(18) };	// 心跳机制，60秒检测一次

	// request get返回的url
	std::string _get_url;
	// request get返回的参数
	std::unordered_map<std::string, std::string> _get_params;
};

