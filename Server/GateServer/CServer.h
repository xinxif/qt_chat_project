#ifndef _CSERVER_H
#define _CSERVER_H
#include"const.h"

class CServer :public std::enable_shared_from_this<CServer>
{
public:
	//alt + enter 选择生成定义
	CServer(boost::asio::io_context& ioc, const unsigned short port);
	void Start();
private:
	

	tcp::acceptor  _acceptor;
	//没有拷贝构造函数
	net::io_context& _ioc;

	boost::asio::ip::tcp::socket   _socket;
};

#endif // !_CSERVER_H
