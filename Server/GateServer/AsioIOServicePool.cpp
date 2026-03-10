#include "AsioIOServicePool.h"



boost::asio::io_context& AsioIOServicePool::GetIOService()
{
	std::size_t index = _nextIOService.fetch_add(1) % _ioServices.size();
	return _ioServices[index];
}

void AsioIOServicePool::Stop()
{
	for (auto& work : _works)
	{
		// 释放守卫。
		// 此时 io_context 发现没有 work 守着了，
		// 它会检查：还有没有正在进行的 async_read 或 async_write？
		// 如果有，它会继续跑；如果没有，run() 立即结束。
		work.reset();
	}
	for (auto& t : _threads)
	{
		if (t.joinable())
		{
			t.join();
		}
	}
}

AsioIOServicePool::AsioIOServicePool(const std::size_t& size):_ioServices(size), _works(size), _threads(), _nextIOService(0)
{
	for (std::size_t i = 0; i < size; ++i)
	{
		//增加一个“工作持有者”，防止事件循环在没有异步任务时立即退出
		_works[i] = std::unique_ptr<Work>(new Work(_ioServices[i].get_executor()));

		_threads.emplace_back([this, i]() {_ioServices[i].run(); });
	}

}
AsioIOServicePool::~AsioIOServicePool()
{
	Stop();
	std::cout << "AsioIOServicePool destruct" << std::endl;
}