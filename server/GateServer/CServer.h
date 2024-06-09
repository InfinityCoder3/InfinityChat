#pragma once

#include "const.h"
#include "HttpConnection.h"

class CServer : public std::enable_shared_from_this<CServer>
{
public:
	//io_contextû���ṩ�������죬��ʹ����������
	CServer(boost::asio::io_context& ioc, unsigned short& port);

	void Start();

private:
	tcp::acceptor _acceptor;
	net::io_context& _ioc;	//io_contextû���ṩ�������죬��ʹ����������
	//tcp::socket _socket;
};

