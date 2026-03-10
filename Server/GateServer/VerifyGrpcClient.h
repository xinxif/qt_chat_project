#ifndef _VERIFYGRPCCLIENT_H_
#define _VERIFYGRPCCLIENT_H_

#include <grpcpp/grpcpp.h>
#include "message.grpc.pb.h"
#include "const.h"
#include "Singleton.h"

using grpc::Channel;
using grpc::Status;
using grpc::ClientContext;

using message::GetVarifyReq;
using message::GetVarifyRsp;
using message::VarifyService;

class RPConPool
{
public:
	RPConPool(const std::size_t poolSize, const std::string& host, const std::string& port);
	std::unique_ptr<VarifyService::Stub> getConnection();
	void recycleConnection(std::unique_ptr<VarifyService::Stub> context);

	~RPConPool();
private:

	void Close();


	std::atomic<bool> b_stop_;
	std::size_t poolSize_;
	std::string host_;
	std::string port_;
	std::queue<std::unique_ptr<VarifyService::Stub>> connections_;
	std::mutex mutex_;
	std::condition_variable cond_;
};


class VerifyGrpcClient :public Singleton<VerifyGrpcClient>
{
	friend class Singleton<VerifyGrpcClient>;
public:

	GetVarifyRsp GetVarifyCode(const std::string &email);

private:
	VerifyGrpcClient();

	std::unique_ptr<RPConPool> pool_;
};


#endif // !_VERIFYGRPCCLIENT_H_

