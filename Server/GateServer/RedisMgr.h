#ifndef _REDISMGR_H_
#define _REDISMGR_H_

#include "Singleton.h"
#include "hiredis.h"
#include <memory>
#include <initializer_list>
#include <atomic>

class RedisConPool:public std::enable_shared_from_this<RedisConPool>
{
public:
    RedisConPool(const std::string& host, const int port, const std::string& pwd
                ,const std::size_t pool_size = std::thread::hardware_concurrency());
    
    ~RedisConPool();
    std::shared_ptr<redisContext> getConnection();
    void Close();
private:
    redisContext* connect(const std::string& host, const int port, const std::string& pwd);
    void returnConnection(redisContext* context);

    std::atomic<bool> b_stop_;
    std::size_t poolSize_;
    std::string host_;
    int port_;
    std::queue<redisContext*> connections_;
    std::mutex mutex;
    std::condition_variable cond_;
};
class RedisMgr: public Singleton<RedisMgr>
                //,public std::enable_shared_from_this<RedisMgr>
{
 
    friend class Singleton<RedisMgr>;

                                 //左侧push
	enum command { GET, SET, AUTH, LPUSH, LPOP, RPUSH, RPOP, HSET, HGET, DEL, EXISTS };
public:
    ~RedisMgr();
    //bool Connect(const std::string& host,const int port);
    bool Get(const std::string& key, std::string& value);
    bool Set(const std::string& key, const std::string& value);
    //bool Auth(const std::string& password);
    bool LPush(const std::string& key, const std::string& value);
    bool LPop(const std::string& key, std::string& value);
    bool RPush(const std::string& key, const std::string& value);
    bool RPop(const std::string& key, std::string& value);
    bool HSet(const std::string& key, const std::string& hkey, const std::string& value);
    std::string HGet(const std::string& key, const std::string& hkey);
    bool Del(const std::string& key);
    bool ExistsKey(const std::string& key);
    void Close();
private:
    std::pair <bool, std::string> doCommand(const command cnd, std::initializer_list<std::string> argv);
    RedisMgr();
    /*
    * redis 主要成员
    typedef struct redisContext {
    int err;            // 错误码
    char errstr[128];   // 错误信息
    int fd;             // socket fd
    }redisContext;
    */
    //redisContext* _connect;
    /*
    typedef struct redisReply {
    int type;                       返回类型 
    long long integer;              对于整数型回复 
    int len;                        字符串长度 
    char* str;                      对应字符串或状态信息 
    size_t elements;                对于数组回复 
    struct redisReply** element;    数组元素指针 
    } redisReply;
    */
    //redisReply* _reply;

    std::unique_ptr<RedisConPool> con_pool_;
};
#endif
