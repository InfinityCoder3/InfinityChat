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
	// �������
	void CheckDeadline();
	// http����Ϣ
	void WriteResponce();
	// http����������Ϣ
	void HandleReq();
	// ��������
	void PreParseGetParam();
private:
	tcp::socket _socket;	//����ͨ�ŵ�socket
	beast::flat_buffer _buffer{ 8192 };		//�������ݻ���
	http::request<http::dynamic_body> _request;		// request message
	http::response<http::dynamic_body> _responce;	// responce message
	// �ײ�ʵ����Ҫȥ��ѯ �󶨵ײ�ĵ�����
	net::steady_timer _deadline{ _socket.get_executor(), std::chrono::seconds(18) };	// �������ƣ�60����һ��

	// request get���ص�url
	std::string _get_url;
	// request get���صĲ���
	std::unordered_map<std::string, std::string> _get_params;
};

