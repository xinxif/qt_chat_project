#ifndef _ASIOIOSERVICEPOOL_H_
#define _ASIOIOSERVICEPOOL_H_

#include <vector>
#include <boost/asio.hpp>
#include "Singleton.h"
class AsioIOServicePool :public Singleton<AsioIOServicePool>
{
    friend Singleton<AsioIOServicePool>;
public:

    using IOService = boost::asio::io_context;
    using Work = boost::asio::executor_work_guard<boost::asio::io_context::executor_type>;
    using WorkPtr = std::unique_ptr<Work>;

    ~AsioIOServicePool();
    AsioIOServicePool(const AsioIOServicePool&) = delete;
    AsioIOServicePool& operator=(const AsioIOServicePool&) = delete;
    // 使用 round-robin 的方式返回一个 io_service
    boost::asio::io_context& GetIOService();
    void Stop();


private:
                                            //获取当前CPU核心数量
    AsioIOServicePool(const std::size_t &size = std::thread::hardware_concurrency());
    std::vector<IOService>      _ioServices;
    std::vector<WorkPtr>        _works;
    std::vector<std::thread>    _threads;

    //下一个IOService的索引
    std::atomic<std::size_t>    _nextIOService;
};

#endif // !_ASIOIOSERVICEPOOL_H_


