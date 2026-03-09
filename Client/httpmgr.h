#ifndef HTTPMGR_H
#define HTTPMGR_H

#include <QObject>
#include<QNetworkAccessManager>     //整个 Qt 网络模块的核心。不需要为每一个请求都创建一个管理器，通常整个程序（或一个长生命周期的对象）只需要一个 QNetworkAccessManager
                                    //异步的。这意味着当你发出请求后，程序不会卡在那里等服务器回消息，而是通过“信号与槽”机制，在收到回复时通知你。

#include<QUrl>                      //地址格式化工具，QUrl 就是专门用来处理和验证“网址”的，自动处理编码（比如把空格转成 %20）、解析端口号、路径和查询参数
#include<QJsonObject>               //信件内容（结构化数据）
#include<QJsonDocument>             //打包与拆包工具
#include<QString>
#include<QByteArray>
#include"singleton.h"
#include"global.h"
                                //c++ 类模板CRTP特性
class HttpMgr : public QObject,public Singleton<HttpMgr>,
                public std::enable_shared_from_this<HttpMgr>
{
    Q_OBJECT
    //让singleton访问httpmgr的构造函数
    friend class Singleton<HttpMgr>;
public:
    ~HttpMgr()=default;
    //地址，数据，具体功能ID，模块
    void post_http_req(const QUrl& url,const QJsonObject &json,const ReqId id,const Modules mod);
private:

    HttpMgr();


    QNetworkAccessManager manager;

private slots:

    //通知注册模块http请求完成
    void slot_http_finish(const ReqId id,const QString &res, const ErrorCodes err,const Modules mod);

signals:
    //http请求发送
    void sig_http_finish(const ReqId id,const QString&result,const ErrorCodes err,const Modules mod);


    void sig_reg_mod_finish(const ReqId id,const QString &res,const ErrorCodes err);
};

#endif // HTTPMGR_H
