#ifndef _CONST_H_
#define _CONST_H_
#include <boost/beast/http.hpp>
#include <boost/beast.hpp>
#include <boost/asio.hpp>

#include <json/json.h>
#include <json/value.h>
#include <json/reader.h>

#include <functional>
#include <iostream>
#include <memory>


#include <mutex>
#include <condition_variable>
#include <atomic>

#include <map>
#include <unordered_map>
#include <string>
#include <queue>

constexpr const char* CODE_PREFIX = "code_";

namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>

enum class ErrorCodes :int
{
	Success = 0,
	Error_Json = 1001,  //JsonΩ‚Œˆ¥ÌŒÛ
	RPCFailed = 1002,  //RPC«Î«Û¥ÌŒÛ
	VarifyExpired = 1003,
	VarifyCodeErr = 1004,
	UserExist = 1005,
	PasswdErr = 1006,
	EmailNotMatch = 1007,
	PasswdUpFailed = 1008,
	passwdInvalid = 1009
};
#endif

