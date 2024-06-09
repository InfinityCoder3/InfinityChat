#include "CServer.h"

CServer::CServer(boost::asio::io_context& ioc, unsigned short& port)
	:_ioc(ioc), _acceptor(ioc,tcp::endpoint(tcp::v4(), port))
{
}

void CServer::Start()
{
	auto self = shared_from_this();
	//auto& ioc = AsioIOContexPool::GetInstance()->GetIOContext();
	//std::shared_ptr<
	tcp::socket sock(_ioc);
	std::shared_ptr<HttpConnection> new_con = std::make_shared<HttpConnection>(_ioc);

	_acceptor.async_accept(new_con->GetSocket(), [self, new_con](beast::error_code ec) {
		try {
			// ����ֱ�ӷ��أ�����������
			if (ec) {
				self->Start();
				return;
			}

			// �ɹ�
			// �����µ�����
			new_con->Start();
			// ��������
			self->Start();
		}
		catch (const std::exception& e){
			std::cerr << "in function " << __func__ << e.what() << std::endl;
		}
		});
}
