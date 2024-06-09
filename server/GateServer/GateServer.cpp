#include "const.h"
#include "CServer.h"



int main()
{
	std::cout << "this is Infnity Chat, welcome!\n";
	std::cout << "Gate Server is running...\n";
	unsigned short port = 8080;
	net::io_context ioc;
	// ��׽�˳��ź�
	boost::asio::signal_set signals(ioc, SIGINT, SIGTERM);
	signals.async_wait([&ioc](const boost::system::error_code& error, int signal_number) {
		if (error) {
			return;
		}
		ioc.stop();
		});

	
	//CServer server(ioc, port);
	// ��Ӧ�̳���enbale_shared_from_this<>���࣬�������ʱ����Ҫʹ��ֻ��ָ�����ʽ���죬
	// ������ʹ����ָ���ֱ����ջ�Ϲ��죬������׳�`_Throw_bad_weak_ptr`����
	auto server = std::make_shared<CServer>(ioc, port);
	//std::make_shared<CServer>(ioc, port);
	server->Start();
	ioc.run();

	std::cout << "Gate Server was exit...\n";
	return 0;
}