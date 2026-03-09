#ifndef _LOGICSYSTEM_H_
#define _LOGICSYSTEM_H_
#include"const.h"
#include"Singleton.h"


class HttpConnection;



class LogicSystem :public Singleton<LogicSystem>
{
	friend class Singleton<LogicSystem>;
public:

	using HttpHandler = std::function<void(std::shared_ptr<HttpConnection>)>;


	~LogicSystem() = default;
	bool HandleGet(const std::string& path, std::shared_ptr<HttpConnection>);
	void RegGet(const std::string& url, HttpHandler handler);
private:
	LogicSystem();
	std::map<std::string, HttpHandler> _post_handlers;
	std::map<std::string, HttpHandler>_get_handlers;
};

#endif // !_LOGICSYSTEM_H_
