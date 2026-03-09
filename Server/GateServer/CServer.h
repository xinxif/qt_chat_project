#ifndef _CSERVER_H
#define _CSERVER_H
#include"const.h"
class CServer :public std::enable_shared_from_this<CServer>
{
public:
	//alt + enter 添加实现
	CServer(boost::asio::io_context& ioc, unsigned short& port);
	void Start();
private:
	
private:
	//注意这三个的初始化顺序
	tcp::acceptor  _acceptor;
	net::io_context& _ioc;
	boost::asio::ip::tcp::socket  _socket;
};

#endif // !_CSERVER_H
