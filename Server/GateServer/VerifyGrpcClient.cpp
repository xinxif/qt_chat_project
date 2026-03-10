#include "VerifyGrpcClient.h"
#include"ConfigMgr.h"

RPConPool::RPConPool(const std::size_t poolSize, const std::string& host, const std::string& port)
	:poolSize_(poolSize), host_(host), port_(port), b_stop_(false)
{
	//建立一个真正的物理通道（共享）
	std::shared_ptr<Channel> channel = grpc::CreateChannel(host + ":" + port, grpc::InsecureChannelCredentials());
	for (std::size_t i = 0; i < poolSize; ++i)
	{
		//复用上面的 channel
		connections_.push(VarifyService::NewStub(channel));
	}

}
std::unique_ptr<VarifyService::Stub> RPConPool::getConnection()
{
	//可以手动 lock、unlock，延迟锁定（使用 std::defer_lock）
	//须配合std::condition_variable使用
	//可以将锁的所有权传给其他函数
	std::unique_lock<std::mutex> lock(mutex_);
	cond_.wait(lock, [this]()
		{
			//没有关闭，或者连接池不为空
			return b_stop_ || !connections_.empty();
		});

	//如果停止则直接返回空指针
	if (b_stop_ && connections_.empty())
		return nullptr;

	auto context = std::move(connections_.front());
	connections_.pop();
	return context;
}
void RPConPool::recycleConnection(std::unique_ptr<VarifyService::Stub> context)
{
	std::lock_guard<std::mutex> lock(mutex_);
	if (b_stop_)
	{
		return;
	}
	connections_.push(std::move(context));
	cond_.notify_one();
}
RPConPool::~RPConPool()
{
	std::lock_guard<std::mutex> lock(mutex_);
	Close();
	while (!connections_.empty())
	{
		connections_.pop();
	}
}
void RPConPool::Close()
{
	b_stop_ = true;
	cond_.notify_all();
}








GetVarifyRsp VerifyGrpcClient::GetVarifyCode(const std::string &email)
{
	//准备好上下文（比如设置超时时间等，这里没设置就是默认）
	ClientContext context;
	GetVarifyRsp reply;         // 准备一个空包裹，用来装服务器返回的结果
	GetVarifyReq request;       // 准备发送给服务器的包裹
	request.set_email(email);   // 把你要验证的邮箱塞进包裹里

	//通过 stub_ (代理人) 发送请求，程序会在这里等待服务器回复
	auto stub_ = pool_->getConnection();
	Status status = stub_->GetVarifyCode(&context, request, &reply);

	if (status.ok())
	{
		pool_->recycleConnection(std::move(stub_));
		return reply;
	}
	else
	{
		pool_->recycleConnection(std::move(stub_));
		reply.set_error(static_cast<int>(ErrorCodes::RPCFailed));
		return reply;
	}
}
VerifyGrpcClient::VerifyGrpcClient() :pool_(nullptr)
{
	auto& gCfgMgr = ConfigMgr::Inst();
	std::string host = gCfgMgr["VarifyServer"]["Host"];
	std::string port = gCfgMgr["VarifyServer"]["Port"];

	pool_.reset(new RPConPool(5, host, port));
}