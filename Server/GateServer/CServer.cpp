#include "CServer.h"
#include"HttpConnection.h"
#include<iostream>
CServer::CServer(boost::asio::io_context& ioc, unsigned short& port) :_ioc(ioc),
    _acceptor(ioc, tcp::endpoint(tcp::v4(), port)), _socket(ioc) 
    //_acceptor(ioc, tcp::v4(), port), _ioc(ioc), _socket(ioc)
    //错误的初始化顺序导致了
    //Error: assign: 句柄无效。 [system:6 at D:\Project\VS\boost_1_90\include\boost\asio\detail\win_iocp_socket_service.hpp:225:5 in function 'assign']
    //因为_acceptor先用到了ioc，所以ioc要先初始化
{

}
void CServer::Start()
{
    auto self = shared_from_this();
    _acceptor.async_accept(_socket, [self](beast::error_code ec) {
        try {
            //出错则放弃这个连接，继续监听新链接
            if (ec) {
                self->Start();
                return;
            }

            //处理新链接，创建HpptConnection类管理新连接
            std::make_shared<HttpConnection>(std::move(self->_socket))->Start();
            //继续监听
            self->Start();
        }
        catch (std::exception& exp) {
            std::cout << "exception is " << exp.what() << std::endl;
            self->Start();
        }
        });
}