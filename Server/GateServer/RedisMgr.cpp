#include "RedisMgr.h"
#include<initializer_list>
RedisMgr::RedisMgr() :_connect(NULL), _reply(NULL)
{

}

RedisMgr::~RedisMgr()
{

}

bool RedisMgr::Connect(const std::string& host, const int port)
{
	//返回成功时，返回一个 有效的 redisContext*，并且 _connect->err == 0
	//返回 NULL 表示 内存分配失败
	//返回对象但 err != 0 说明连接失败
	this->_connect = redisConnect(host.c_str(), port);

	if (_connect == NULL || _connect->err)
	{

		if (_connect)
		{
			printf("Connection error: %s\n", _connect->errstr);
			redisFree(_connect);
			return false;
		}
		else
		{
			printf("Connection error: can't allocate redis context\n");
			return false;
		}
	}
	return true;
}

bool RedisMgr::Auth(const std::string& password)						{return doCommand(AUTH, { password }).first;}
bool RedisMgr::Set(const std::string& key, const std::string& value)	{return doCommand(SET, { key,value }).first;}
bool RedisMgr::LPush(const std::string& key, const std::string& value)	{return doCommand(LPUSH, { key,value }).first;}
bool RedisMgr::RPush(const std::string& key, const std::string& value)	{return doCommand(RPUSH, { key,value }).first; }
bool RedisMgr::HSet(const std::string& key, const std::string& hkey, const std::string& value) {return doCommand(HSET, { key,hkey,value }).first;}
std::string RedisMgr::HGet(const std::string& key, const std::string& hkey)					   {return doCommand(HGET, { key,hkey }).second;}
bool RedisMgr::Del(const std::string& key)		 {return doCommand(DEL, { key }).first;}
bool RedisMgr::ExistsKey(const std::string& key) {return doCommand(EXISTS, { key }).first;}
void RedisMgr::Close()	{redisFree(_connect);}

bool RedisMgr::Get(const std::string& key, std::string& value)
{
	auto ret = doCommand(GET, { key });
	if (ret.first) {
		value = ret.second;   // 将返回值赋给传入引用
	}
	return ret.first;         // 返回执行成功/失败
}

bool RedisMgr::LPop(const std::string& key, std::string& value)
{
	auto ret = doCommand(LPOP, { key });
	if (ret.first) {
		value = ret.second;
	}
	return ret.first;
}

bool RedisMgr::RPop(const std::string& key, std::string& value)
{
	auto ret = doCommand(RPOP, { key });
	if (ret.first) {
		value = ret.second;
	}
	return ret.first;
}

std::pair <bool,std::string> RedisMgr::doCommand(const command cnd, std::initializer_list<std::string> argv)
{
/*
	| 宏                     | 值 | 说明                                         |
	| --------------------- | - | --------------------------------              |
	| `REDIS_REPLY_STRING`  | 1 | 普通字符串回复（如 `GET key` 返回的字符串）      |
	| `REDIS_REPLY_ARRAY`   | 2 | 数组回复（如 `LRANGE` 返回多个元素）             |
	| `REDIS_REPLY_INTEGER` | 3 | 整数回复（如 `INCR key`）                       |
	| `REDIS_REPLY_NIL`     | 4 | 空值回复（如不存在的 key 返回 `nil`）            |
	| `REDIS_REPLY_STATUS`  | 5 | 状态回复（如 `SET key value` 返回 `"OK"`）      |
	| `REDIS_REPLY_ERROR`   | 6 | 错误回复（如 `SET key` 没有权限或语法错误）      |
*/

	std::pair<bool, std::string> ret;
	switch (cnd)
	{
	case GET:
	{
		std::string key(*argv.begin());
		this->_reply = static_cast<redisReply*>(redisCommand(this->_connect, "GET %s",key.data()));
		if (this->_reply == NULL) 
		{
			std::cout << "[ GET  " << key << " ] failed" << std::endl;
			ret.first = false;
			break;
		}
		if (this->_reply->type != REDIS_REPLY_STRING) 
		{
			std::cout << "[ GET  " << key << " ] failed" << std::endl;
			ret.first = false;
			break;
		}
		ret.first = true;
		ret.second.assign(this->_reply->str);
		std::cout << "Succeed to execute command [ GET " << key << "  ]" << std::endl;
		break;
	}
	case SET:
	{
		auto key = argv.begin();
		auto value = argv.begin() + 1;
		this->_reply = static_cast<redisReply*>(redisCommand(this->_connect, "SET %s %s", key->c_str(), value->c_str()));
		//如果返回NULL则说明执行失败
		if (NULL == this->_reply)
		{
			std::cout << "Execut command [ SET " << *key << "  " << *value << " ] failure ! " << std::endl;
			ret.first = false;
			break;
		}
		//如果执行失败则释放连接
		if (!(this->_reply->type == REDIS_REPLY_STATUS &&
			(strcmp(this->_reply->str, "OK") == 0 || strcmp(this->_reply->str, "ok") == 0)))
		{
			std::cout << "Execut command [ SET " << *key << "  " << *value << " ] failure ! " << std::endl;
			ret.first = false;
			break;
		}
		//执行成功 释放redisCommand执行后返回的redisReply所占用的内存
		std::cout << "Execut command [ SET " << *key << "  " << *value << " ] success ! " << std::endl;
		ret.first = true;
		break;
	}
	case AUTH:
	{
		
		this->_reply = static_cast<redisReply*>(redisCommand(this->_connect, "AUTH %s", argv.begin()->data()));
		if (this->_reply->type == REDIS_REPLY_ERROR)
		{
			std::cout << "认证失败\n";
			ret.first = false;
		}
		else
		{
			std::cout << "认证成功\n";
			ret.first = true;
		}
		break;
	}
	case LPUSH:
	{
		auto key = argv.begin();
		auto value = argv.begin() + 1;
		this->_reply = (redisReply*)redisCommand(this->_connect, "LPUSH %s %s", key->c_str(), value->c_str());
		if (NULL == this->_reply)
		{
			std::cout << "Execut command [ LPUSH " << *key << "  " << *value << " ] failure ! " << std::endl;
			ret.first = false;
			break;
		}
		if (this->_reply->type != REDIS_REPLY_INTEGER || this->_reply->integer <= 0) {
			std::cout << "Execut command [ LPUSH " << *key << "  " << *value << " ] failure ! " << std::endl;
			ret.first = false;
			break;
		}
		std::cout << "Execut command [ LPUSH " << *key << "  " << *value << " ] success ! " << std::endl;
		ret.first = true;
		break;
	}
	case LPOP:
	{
		auto key = argv.begin();
		this->_reply = (redisReply*)redisCommand(this->_connect, "LPOP %s ", key->c_str());
		if (_reply == nullptr || _reply->type == REDIS_REPLY_NIL) 
		{
			std::cout << "Execut command [ LPOP " << *key << " ] failure ! " << std::endl;
			ret.first = false;
			break;
		}
		ret.second = _reply->str;
		ret.first = true;
		std::cout << "Execut command [ LPOP " << *key << " ] success ! " << std::endl;
		break;
	}

	case RPUSH:
	{
		auto key = argv.begin();
		auto value = argv.begin() + 1;
		this->_reply = (redisReply*)redisCommand(this->_connect, "RPUSH %s %s", key->c_str(), value->c_str());
		if (NULL == this->_reply)
		{
			std::cout << "Execut command [ RPUSH " << *key << "  " << *value << " ] failure ! " << std::endl;
			ret.first = false;
			break;
		}
		if (this->_reply->type != REDIS_REPLY_INTEGER || this->_reply->integer <= 0) {
			std::cout << "Execut command [ RPUSH " << *key << "  " << *value << " ] failure ! " << std::endl;
			ret.first = false;
			break;
		}
		std::cout << "Execut command [ RPUSH " << *key << "  " << *value << " ] success ! " << std::endl;
		ret.first = true;
		break;
	}
	case RPOP:
	{
		auto key = argv.begin();
		this->_reply = (redisReply*)redisCommand(this->_connect, "RPOP %s ", key->c_str());
		if (_reply == nullptr || _reply->type == REDIS_REPLY_NIL) {
			std::cout << "Execut command [ RPOP " << *key << " ] failure ! " << std::endl;
			ret.first = false;
			break;
		}
		ret.second = _reply->str;
		ret.first = true;
		std::cout << "Execut command [ RPOP " << *key << " ] success ! " << std::endl;
		break;
	}
	case HSET:
	{
		const char* cmd_argv[4]{ "HSET" };
		std::size_t cmd_argvlen[4]{ 4 };
		auto beg = argv.begin();

		for (std::size_t i = 1; beg != argv.end(); ++beg,++i)
		{
			cmd_argv[i] = beg->data();
			cmd_argvlen[i] = beg->length();
		}

		this->_reply = (redisReply*)redisCommandArgv(this->_connect, argv.size()+1, cmd_argv, cmd_argvlen);

		if (this->_reply == nullptr || this->_reply->type != REDIS_REPLY_INTEGER)
		{
			std::cout << "Execute command [ HSET "<< cmd_argv[1] << " "<< cmd_argv[2] << " "<< cmd_argv[3] << " ] failure!\n";
			ret.first = false;
			break;
		}

		std::cout << "Execute command [ HSET " << cmd_argv[1] << " " << cmd_argv[2] << " " << cmd_argv[3] << " ] success!\n";
		ret.first = true;
		break;
	}

	case HGET:
	{
		const char* cmd_argv[3]{ "HGET" };
		std::size_t cmd_argvlen[3]{ 4 };

		auto beg = argv.begin();

		for (std::size_t i = 1; beg != argv.end(); ++beg, ++i)
		{
			cmd_argv[i] = beg->data();
			cmd_argvlen[i] = beg->length();
		}

		this->_reply = (redisReply*)redisCommandArgv(this->_connect,argv.size()+1, cmd_argv, cmd_argvlen);

		if (this->_reply == nullptr || this->_reply->type == REDIS_REPLY_NIL)
		if (this->_reply == nullptr || this->_reply->type == REDIS_REPLY_NIL)
		if (this->_reply == nullptr || this->_reply->type == REDIS_REPLY_NIL)
		{
			std::cout << "Execute command [ HGET "<< cmd_argv[1] << " "<< cmd_argv[2] << " ] failure!\n";
			ret.first = false;
			break;
		}

		if (this->_reply->type != REDIS_REPLY_STRING)
		{
			std::cout << "Execute command [ HGET "<< cmd_argv[1] << " "<< cmd_argv[2] << " ] failure!\n";
			ret.first = false;
			break;
		}
		ret.first = true;
		ret.second.assign(this->_reply->str);
		std::cout << "Execute command [ HGET "<< cmd_argv[1] << " "<< cmd_argv[2] << " ] success!\n";
		break;
	}
	case DEL:
	{
		auto key = argv.begin();
		this->_reply = (redisReply*)redisCommand(this->_connect, "DEL %s", key->c_str());
		if (this->_reply == nullptr || this->_reply->type != REDIS_REPLY_INTEGER) 
		{
			std::cout << "Execut command [ Del " << *key << " ] failure ! " << std::endl;
			ret.first = false;
			break;
		}
		std::cout << "Execut command [ Del " << *key << " ] success ! " << std::endl;
		ret.first = true;
		break;
	}
	case EXISTS:
	{
		auto key = argv.begin();
		this->_reply = (redisReply*)redisCommand(this->_connect, "exists %s", key->c_str());
		if (this->_reply == nullptr || this->_reply->type != REDIS_REPLY_INTEGER || this->_reply->integer == 0) {
			std::cout << "Not Found [ Key " << *key << " ]  ! " << std::endl;
			ret.first = false;
			break;
		}
		std::cout << " Found [ Key " << *key << " ] exists ! " << std::endl;
		ret.first = true;
		break;
	}
	default:
		std::cout << "Unknown command" << std::endl;
		break;
	}
	freeReplyObject(this->_reply);
	this->_reply = NULL;

	return ret;
}