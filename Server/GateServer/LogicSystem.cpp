#include"HttpConnection.h"
#include"LogicSystem.h"
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

}

void LogicSystem::RegGet(const std::string& url, HttpHandler handler)
{
	_get_handlers.insert(std::make_pair(url, handler));

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
