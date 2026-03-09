#include"CServer.h"
#include"ConfigMgr.h"
ConfigMgr gCfgMgr;

int main()
{
    
    unsigned short gate_port = atoi(gCfgMgr["GateServer"]["Port"].c_str());
    try
    {
        net::io_context ioc{ 1 };
        boost::asio::signal_set signals(ioc, SIGINT, SIGTERM);
        signals.async_wait([&ioc](const boost::system::error_code& error, int signal_number) {

            if (error) {
                return;
            }
            ioc.stop();
            });
        std::make_shared<CServer>(ioc, gate_port)->Start();
        std::cout << "GateServer start:" << gate_port << "\n";
        ioc.run();
    }
    catch (std::exception const& e)
    {
        std::cerr << "Error: " << e.what() << "\n";
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}