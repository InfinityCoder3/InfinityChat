#pragma once

#include "const.h"
#include "HttpConnection.h"

class CServer : public std::enable_shared_from_this<CServer>
{
public:
	//io_context没有提供拷贝构造，故使用引用类型
	CServer(boost::asio::io_context& ioc, unsigned short& port);

	void Start();

private:
	tcp::acceptor _acceptor;
	net::io_context& _ioc;	//io_context没有提供拷贝构造，故使用引用类型
	//tcp::socket _socket;
};

