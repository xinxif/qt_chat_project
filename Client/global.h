#ifndef GLOBAL_H
#define GLOBAL_H
#include <functional>
#include <QWidget>
#include "QStyle"

//Qt 在运行时修改动态属性后： 样式不会自动刷新，需要手动刷新
extern std::function<void(QWidget*)> repolish;
extern QString gate_url_prefix;
enum class ReqId
{
    ID_GET_VERIFY_CODE = 1001,  //获取验证码
    ID_REG_USER = 1002,         //注册用户
};
enum class Modules
{
    REGESTERMOD = 0,
};

enum class ErrorCodes
{
    SUCCESS = 0,            //
    ERR_JSON = 1,           //json解析失败
    ERR_NETWORK = 2,        //网络错误
};

#endif // GLOBAL_H
