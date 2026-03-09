#include"HttpConnection.h"
#include"LogicSystem.h"

/*
	tcp::socket _socket;
	beast::flat_buffer _buffer{ 2048 };
	http::request<http::dynamic_body> _request;
	http::response<http::dynamic_body> _response;
	net::steady_timer deadline_;
*/
HttpConnection::HttpConnection(tcp::socket socket)
	:_socket(std::move(socket)), _buffer(), _request(), _response(),
	deadline_(_socket.get_executor(), std::chrono::seconds(60))

{

}
void HttpConnection::Start()
{
	auto self = shared_from_this();
	http::async_read(_socket, _buffer, _request, [self](beast::error_code ec,
		std::size_t bytes_transferred)
		{
			try
			{
				if (ec)
				{
					std::cout << "http read err is " << ec.what() << "\n";
					return;
				}

				boost::ignore_unused(bytes_transferred);
				self->HandleReq();
				self->CheckDeadline();
			}
			catch (std::exception& exp) {
				std::cout << "exception is " << exp.what() << "\n";
			}
		}
	);
}
void HttpConnection::CheckDeadline()
{
	auto self = shared_from_this();
	deadline_.async_wait([self](beast::error_code cd) {
		if (!cd)
		{
			self->_socket.close();	//socket 立刻被销毁	所有未完成的异步操作会被取消 底层文件描述符被释放 这个 socket 对象不能再继续使用
		}
		});
}
void HttpConnection::WriteRespone()
{
	auto self = shared_from_this();
	_response.content_length(_response.body().size());	//HTTP 头部中写入 Content-Length 字段，告诉客户端 后面的 body 有多少字节
	http::async_write(_socket, _response, [self](beast::error_code ec, std::size_t bytes_transferred)
		{
			self->_socket.shutdown(tcp::socket::shutdown_send, ec);	//shutdown关闭通信方向
			self->deadline_.cancel();
		});

}
void HttpConnection::HandleReq()
{
	_response.version(_request.version());
	_response.keep_alive(false);		//设置短链接

	auto method = _request.method();

	switch (method)
	{
	case http::verb::get:
	{														//域名/地址
		bool success = LogicSystem::GetInstance()->HandleGet(_request.target(), shared_from_this());
		if (!success)
		{
			_response.result(http::status::not_found);				//HTTP/1.1 404 Not Found
			_response.set(http::field::content_type, "text/plain");//http::field::date, get_http_date()，普通文件，设计回复日期
			beast::ostream(_response.body()) << "url not found\r\n";
			WriteRespone();	//写回复
			break;
		}

		_response.result(http::status::ok);
		_response.set(http::field::server, "GateServer");
		WriteRespone();
		break;
	}
	default:
		break;
	}
}