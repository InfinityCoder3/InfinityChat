#pragma once

/*
 * 这是一个用于包含常用头文件的头文件
 */

#include <boost/beast/http.hpp> //http
#include <boost/beast.hpp> 
#include <boost/asio.hpp>    //asio
#include <memory>		//smart_pointer enable_shared_from_this
#include <iostream>
#include <functional>
#include <map>
#include <unordered_map>
#include <json/json.h>
#include <json/value.h>
#include <json/reader.h>
#include <boost/filesystem.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <atomic>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <boost/asio/ip/tcp.hpp>
#include "Singleton.h"

namespace beast = boost::beast;
namespace http = boost::beast::http;
namespace net = boost::asio;
using tcp = boost::asio::ip::tcp;

enum ErrorCodes
{
	Success = 0,
	Error_Json = 1001,	//Json解析错误
	RPCFailed = 1002	// RPC请求错误
};