#ifndef _HTTP_CONNECTION_H_
#define _HTTP_CONNECTION_H_
#include"const.h"
class LogicSystem;
class HttpConnection : public std::enable_shared_from_this<HttpConnection>
{
	friend class LogicSystem;
public:
	HttpConnection(tcp::socket socket);
	void Start();
private:
	void CheckDeadline();
	void WriteRespone();
	void HandleReq();
	void PreParseGetParam();
	tcp::socket _socket;
	beast::flat_buffer _buffer{ 2048 };
	http::request<http::dynamic_body> _request;
	http::response<http::dynamic_body> _response;
	net::steady_timer deadline_;

	std::string _get_url;
	std::unordered_map<std::string, std::string> _get_params;
};

#endif

