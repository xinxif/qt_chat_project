#include "CServer.h"
#include"HttpConnection.h"
#include<iostream>
CServer::CServer(boost::asio::io_context& ioc,const unsigned short port)
	: _acceptor(ioc, tcp::v4(), port), _ioc(ioc), _socket(ioc)
{

}
void CServer::Start()
{
	auto self = shared_from_this();
	_acceptor.async_accept(_socket, [self](beast::error_code ec) 
	{
			try
			{
				//处理连接错误，继续接受下一个连接
				if (ec)
				{
					self->Start();
					return;
				}
				// 创建HttpConnection对象，传递当前socket
				std::make_shared<HttpConnection>(std::move(self->_socket))->Start();
				// 继续接受下一个连接
				self->Start();
			}
			catch (const std::exception&exp)
			{
				std::cout << "excpetion is" << exp.what() << "\n";
				self->Start();
			}
	});
}