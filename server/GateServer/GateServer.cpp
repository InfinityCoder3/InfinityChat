#include "const.h"
#include "CServer.h"



int main()
{
	std::cout << "this is Infnity Chat, welcome!\n";
	std::cout << "Gate Server is running...\n";
	unsigned short port = 8080;
	net::io_context ioc;
	// 捕捉退出信号
	boost::asio::signal_set signals(ioc, SIGINT, SIGTERM);
	signals.async_wait([&ioc](const boost::system::error_code& error, int signal_number) {
		if (error) {
			return;
		}
		ioc.stop();
		});

	
	//CServer server(ioc, port);
	// 对应继承了enbale_shared_from_this<>的类，构造对象时，需要使用只能指针的形式构造，
	// 而不能使用裸指针或直接在栈上构造，否则会抛出`_Throw_bad_weak_ptr`错误
	auto server = std::make_shared<CServer>(ioc, port);
	//std::make_shared<CServer>(ioc, port);
	server->Start();
	ioc.run();

	std::cout << "Gate Server was exit...\n";
	return 0;
}