#include "HttpConnection.h"
#include "LogicSystem.h"


HttpConnection::HttpConnection(net::io_context& ioc) :_socket(ioc)
{
}

void HttpConnection::Start()
{
	std::cout << __func__  << " " << this << " was run once\n";

	auto self = shared_from_this();
	// �첽��ȡ����
	http::async_read(_socket, _buffer, _request, [self](beast::error_code ec, std::size_t bytes_transferred) {
		try
		{
			if (ec) {
				std::cout << "Http read err is " << ec.what() << std::endl;
				return;
			}

			// ������յ�������
			boost::ignore_unused(bytes_transferred);
			self->HandleReq();
			self->CheckDeadline();
		}
		catch (const std::exception& e)
		{
			std::cout << e.what() << std::endl;
		}
		});
}

void HttpConnection::CheckDeadline()
{
	std::cout << "CheckDeadline run once.\n";
	// ����ʹ��thisָ�룬��ֹ�����ڱ𴦱�������
	auto self = shared_from_this();
	_deadline.async_wait([self](beast::error_code ec) {
		if (!ec) {
			std::cerr << "CheckDeadline close the peer socket.\n";
			std::cerr << "error msaage " << ec.what() << std::endl;
			self->_socket.close(ec);
		}
		});
}

void HttpConnection::WriteResponce()
{
	auto self = shared_from_this();
	_responce.content_length(_responce.body().size());
	http::async_write(_socket, _responce, [self](beast::error_code ec, std::size_t bytes_tansfereed) {
		self->_socket.shutdown(tcp::socket::shutdown_send, ec);
		self->_deadline.cancel();
		});
}

void HttpConnection::HandleReq()
{
	//
	_responce.version(_request.version());
	_responce.keep_alive(false);
	// Get method
	if (_request.method() == http::verb::get) {
		PreParseGetParam();

		bool success = LogicSystem::GetInstance()->HandleGet(_get_url, shared_from_this());
		if (!success) {
			_responce.result(http::status::not_found);
			_responce.set(http::field::content_type, "text/plain");
			beast::ostream(_responce.body()) << "url not found\r\n";
			WriteResponce();
			return;
		}

		_responce.result(http::status::ok);
		_responce.set(http::field::server, "GateServer");
		_responce.set(http::field::content_language, "Chinese");
		WriteResponce();
		return;
	}

	if (_request.method() == http::verb::post) {
		std::cout << "post _get_url " << _get_url << std::endl;
		std::cout << "post request target is " << _request.target() << std::endl;
		bool success = LogicSystem::GetInstance()->HandlePost(_request.target(), shared_from_this());
		std::cout << success << std::endl;
		if (!success) {
			_responce.result(http::status::not_found);
			_responce.set(http::field::content_type, "text/plain");
			beast::ostream(_responce.body()) << "url not found\r\n";
			WriteResponce();
			return;
		}

		_responce.result(http::status::ok);
		_responce.set(http::field::server, "GateServer");
		_responce.set(http::field::content_language, "Chinese");
		_responce.set(http::field::user_agent, "Infinity/0.0.1");
		WriteResponce();
		return;
	}

}

unsigned char ToHex(unsigned char x)
{
	return  x > 9 ? x + 55 : x + 48;
}

unsigned char FromHex(unsigned char x)
{
	unsigned char y;
	if (x >= 'A' && x <= 'Z') y = x - 'A' + 10;
	else if (x >= 'a' && x <= 'z') y = x - 'a' + 10;
	else if (x >= '0' && x <= '9') y = x - '0';
	else assert(0);
	return y;
}

std::string UrlEncode(const std::string& str)
{
	std::string strTemp = "";
	size_t length = str.length();
	for (size_t i = 0; i < length; i++)
	{
		//�ж��Ƿ�������ֺ���ĸ����
		if (isalnum((unsigned char)str[i]) ||
			(str[i] == '-') ||
			(str[i] == '_') ||
			(str[i] == '.') ||
			(str[i] == '~'))
			strTemp += str[i];
		else if (str[i] == ' ') //Ϊ���ַ�
			strTemp += "+";
		else
		{
			//�����ַ���Ҫ��ǰ��%���Ҹ���λ�͵���λ�ֱ�תΪ16����
			strTemp += '%';
			strTemp += ToHex((unsigned char)str[i] >> 4);
			strTemp += ToHex((unsigned char)str[i] & 0x0F);
		}
	}
	return strTemp;
}

std::string UrlDecode(const std::string& str)
{
	std::string strTemp = "";
	size_t length = str.length();
	for (size_t i = 0; i < length; i++)
	{
		//��ԭ+Ϊ��
		if (str[i] == '+') strTemp += ' ';
		//����%������������ַ���16����תΪchar��ƴ��
		else if (str[i] == '%')
		{
			assert(i + 2 < length);
			unsigned char high = FromHex((unsigned char)str[++i]);
			unsigned char low = FromHex((unsigned char)str[++i]);
			strTemp += high * 16 + low;
		}
		else strTemp += str[i];
	}
	return strTemp;
}

void HttpConnection::PreParseGetParam()
{
	// ��ȡurl�еĲ��� get_test?k1=v1&k2=v2
	boost::beast::string_view url = _request.target();
	std::cout << "request url is: " << url << std::endl;
	size_t query_pos = url.find('?');
	// δ�ҵ�
	if (query_pos == std::string::npos) {
		_get_url = url;
		return;
	}

	_get_url = url.substr(0, query_pos);
	std::string query_string = url.substr(query_pos + 1);
	std::string key;
	std::string value;
	size_t pos = 0;
	while ((pos = query_string.find('&')) != std::string::npos) {
		std::string pair = query_string.substr(0, pos);
		size_t eq_pos = pair.find('=');
		if (eq_pos != std::string::npos) {
			key = UrlEncode(pair.substr(0, eq_pos));
			value = UrlEncode(pair.substr(eq_pos + 1));
			_get_params[key] = value;
		}
		query_string.erase(0, pos + 1);
	}
	// �������һ�������ԣ����û�� & �ָ�����  
	if (!query_string.empty()) {
		size_t eq_pos = query_string.find('=');
		if (eq_pos != std::string::npos) {
			key = UrlDecode(query_string.substr(0, eq_pos));
			value = UrlDecode(query_string.substr(eq_pos + 1));
			_get_params[key] = value;
		}
	}
}

