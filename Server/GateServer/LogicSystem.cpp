#include"HttpConnection.h"
#include"LogicSystem.h"
#include"VerifyGrpcClient.h"
LogicSystem::LogicSystem():_post_handlers(), _get_handlers()
{
	RegGet("/get_test", [](std::shared_ptr<HttpConnection> connect) 
		{
			beast::ostream(connect->_response.body()) << "receive get_test req\r\n"; 
			int i = 0;
			for (const auto& elem : connect->_get_params)
			{
				++i;
				beast::ostream(connect->_response.body()) << "param " << i << " key is " << elem.first;
				beast::ostream(connect->_response.body()) << " , " << " value is " << elem.second << "\n";
			}
		});
	RegPost("/get_varifycode", [](std::shared_ptr<HttpConnection> connection) 
	{
		//网络数据往往是分批到达的。_request.body() 里的数据可能分布在内存中好几块不连续的区域里
		//.data() 返回的是一个 “常量缓冲区序列” (ConstBufferSequence)。它本质上是一组指针和长度的集合，告诉程序：“第一段数据在地址 A，长 10 字节；第二段在地址 B，长 20 字节……
		//它会根据 .data() 提供的“地图”，依次访问每一个内存片段，把里面的字节内容拷贝出来，并拼接到一个连续的 std::string 对象中
		auto body_str = boost::beast::buffers_to_string(connection->_request.body().data());

		std::cout << "receive body is " << body_str << std::endl;
		connection->_response.set(http::field::content_type, "text/json");

		Json::Value root;
		//解析器
		Json::Reader reader;
		Json::Value src_root;

		bool parse_success = reader.parse(body_str, src_root);
		if (!parse_success||!src_root.isMember("email")) 
		{
			std::cout << "Failed to parse JSON data!" << std::endl;
			root["error"] = static_cast<int>(ErrorCodes::Error_Json);
			std::string jsonstr = root.toStyledString();
			beast::ostream(connection->_response.body()) << jsonstr;
			return;
		}

		auto email = src_root["email"].toStyledString();

		GetVarifyRsp rsp = VerifyGrpcClient::GetInstance()->GetVarifyCode(email);

		std::cout << "email is " << email << std::endl;
		root["error"] = rsp.error();	//jsoncpp的设计与map类似
		root["email"] = src_root["email"];

		std::string jsonstr = root.toStyledString();

		beast::ostream(connection->_response.body()) << jsonstr;

		return;
	});
}

void LogicSystem::RegGet(const std::string& url, HttpHandler handler)
{
	_get_handlers.insert(std::make_pair(url, handler));

}
void LogicSystem::RegPost(std::string url, HttpHandler handler)
{
	_post_handlers.insert(std::make_pair(url, handler));
}
bool LogicSystem::HandleGet(const std::string& path, std::shared_ptr<HttpConnection> con)
{
	if (_get_handlers.find(path) == _get_handlers.end())
	{
		return false;
	}
	_get_handlers[path](con);
	return true;
}

bool LogicSystem::HandlePost(const std::string &path, std::shared_ptr<HttpConnection> con) {
	if (_post_handlers.find(path) == _post_handlers.end()) {
		return false;
	}

	_post_handlers[path](con);
	return true;
}