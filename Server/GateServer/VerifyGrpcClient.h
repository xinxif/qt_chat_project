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

class VerifyGrpcClient :public Singleton<VerifyGrpcClient>
{
    friend class Singleton<VerifyGrpcClient>;
public:

    GetVarifyRsp GetVarifyCode(std::string email) {
        //准备好上下文（比如设置超时时间等，这里没设置就是默认）
        ClientContext context;
        GetVarifyRsp reply;         // 准备一个空包裹，用来装服务器返回的结果
        GetVarifyReq request;       // 准备发送给服务器的包裹
        request.set_email(email);   // 把你要验证的邮箱塞进包裹里

        //通过 stub_ (代理人) 发送请求，程序会在这里等待服务器回复
        Status status = stub_->GetVarifyCode(&context, request, &reply);

        if (status.ok()) {

            return reply;
        }
        else {
            reply.set_error(static_cast<int>(ErrorCodes::RPCFailed));
            return reply;
        }
    }

private:
    VerifyGrpcClient() 
    {
        std::shared_ptr<Channel> channel = grpc::CreateChannel("127.0.0.1:50051", grpc::InsecureChannelCredentials());
        stub_ = VarifyService::NewStub(channel);
    }

    std::unique_ptr<VarifyService::Stub> stub_;
};


#endif // !_VERIFYGRPCCLIENT_H_

