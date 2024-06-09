#include "LogicSystem.h"
#include "HttpConnection.h"


LogicSystem::LogicSystem()
{
	// 处理Get请求
	RegGet("/get_test", [](std::shared_ptr<HttpConnection> con) {
		// 将收到的数据打印在控制台
		beast::ostream(con->_responce.body()) << "receive get_test req\n";
		std::cout << "receive get_test req\n";
		int i = 0;
		for (auto& elem : con->_get_params) {
			i++;
			beast::ostream(con->_responce.body()) << "param" << i <<
				" key is " << elem.first << " value is " << elem.second << std::endl;
		}
		});

	// 处理Post请求, 可处理文本，图像，音视频等内容
	RegPost("/get_varifycode", [](std::shared_ptr<HttpConnection> con) {
		// 将收到的数据打印在控制台
		std::string body_str = boost::beast::buffers_to_string(con->_request.body().data());
		std::cout << "receive body is " << body_str << std::endl;
		con->_request.set(http::field::content_type, "text/json");
		Json::Value root;
		Json::Reader reader;
		Json::Value src_root;
		bool parse_cuccess = reader.parse(body_str, src_root);
		if (!parse_cuccess) {
			std::cout << "Failed to parse JSON data!\n";
			root["error"] = ErrorCodes::Error_Json;
			std::string jsonStr = root.toStyledString();
			beast::ostream(con->_responce.body()) << jsonStr;
			return false;
		}

		if (!src_root.isMember("email")) {
			std::cout << "JSON data dose not have email!" << std::endl;
			root["error"] = ErrorCodes::Error_Json;
			std::string jsonstr = root.toStyledString();
			beast::ostream(con->_responce.body()) << jsonstr;
			return false;
		}

		auto email = src_root["email"].asString();
		//GetVarifyRsp rsp = VerifyGrpcClient::GetInstance()->GetVarifyCode(email);
		std::cout << "email is " << email << std::endl;
		//root["error"] = rsp.error();
		root["error"] = ErrorCodes::Success;
		root["email"] = src_root["email"];
		std::string jsonstr = root.toStyledString();
		beast::ostream(con->_responce.body()) << jsonstr;
		return true;

		});
}

LogicSystem::~LogicSystem()
{
	std::cout << __func__ << std::endl;
}

bool LogicSystem::HandleGet(std::string path, std::shared_ptr<HttpConnection> con)
{
	if (_get_handlers.find(path) == _get_handlers.end())
		return false;

	_get_handlers[path](con);
	return true;
}

bool LogicSystem::HandlePost(std::string path, std::shared_ptr<HttpConnection> con)
{
	if (_post_handlers.find(path) == _post_handlers.end())
		return false;

	_post_handlers[path](con);
	return true;
}

void LogicSystem::RegGet(std::string url, HttpHandler handler)
{
	//_get_handlers.insert({ url, handler });
	_get_handlers.insert(make_pair(url, handler));
}

void LogicSystem::RegPost(std::string url, HttpHandler handler)
{
	_post_handlers.insert(make_pair( url, handler ));
}

