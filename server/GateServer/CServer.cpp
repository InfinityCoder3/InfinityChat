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
			// 出错，直接返回，并继续监听
			if (ec) {
				self->Start();
				return;
			}

			// 成功
			// 创建新的链接
			new_con->Start();
			// 继续监听
			self->Start();
		}
		catch (const std::exception& e){
			std::cerr << "in function " << __func__ << e.what() << std::endl;
		}
		});
}
