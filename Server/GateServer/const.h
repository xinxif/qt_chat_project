#ifndef _CONST_H_
#define _CONST_H_
#include <boost/beast/http.hpp>
#include <boost/beast.hpp>
#include <boost/asio.hpp>
#include <memory>
#include <iostream>
#include <map>	
#include <functional>
#include <iostream>
#include <mutex>
#include <string>
#include <json/json.h>
#include <json/value.h>
#include <json/reader.h>


namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>

enum class ErrorCodes :int
{
    Success = 0,
    Error_Json = 1001,  //JsonΩ‚Œˆ¥ÌŒÛ
    RPCFailed = 1002,  //RPC«Î«Û¥ÌŒÛ
};

#endif

