#include "httpmgr.h"
#include<QNetworkReply>
HttpMgr::HttpMgr()
{
    //自己的http请求完成，发送信号，自己的槽函数接收
    connect(this, &HttpMgr::sig_http_finish, this, &HttpMgr::slot_http_finish);
}
void HttpMgr::post_http_req(const QUrl &url, const QJsonObject &json, const ReqId id, const Modules mod)
{
    // 1. 将 JSON 对象转换为 QByteArray（字节数组）
    // 网络传输的是二进制数据，所以需要把 QJsonObject 序列化为标准的 JSON 文本格式
    QByteArray data = QJsonDocument(json).toJson();

    // 2. 创建一个网络请求对象，并设置目标 URL
    QNetworkRequest request(url);

    // 3. 设置 HTTP 请求头：告诉服务器我们发送的数据格式是 JSON
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    // 4. 设置 HTTP 请求头：告诉服务器发送的数据长度是多少字节
    request.setHeader(QNetworkRequest::ContentLengthHeader, QByteArray::number(data.length()));

    // 5. 获取当前对象的智能指针（防止在异步等待期间 HttpMgr 被销毁）
    // 这里的 self 会被 Lambda 表达式捕获，确保回调执行时 HttpMgr 依然在内存中
    auto self = this->shared_from_this();

    // 6. 使用 QNetworkAccessManager (manager) 发出 POST 请求
    // 该函数是非阻塞的，会立即返回一个 QNetworkReply 指针，用于跟踪后续的响应状态
    QNetworkReply *reply = manager.post(request, data);

    // 7. 核心：建立信号槽连接。当服务器响应完成（成功或失败）时，会触发 reply 的 finished 信号
    QObject::connect(reply, &QNetworkReply::finished, [self, reply, id, mod]()
    {

        // --- 以下代码在请求完成后（可能在几百毫秒后）异步执行 ---

        // 8. 检查是否有网络层面的错误（如 DNS 解析失败、超时、服务器 404 等）
        if(reply->error() != QNetworkReply::NoError)
        {
            qDebug() << reply->errorString(); // 打印具体的错误信息

            // 9. 发射自定义信号，通知 UI 层：请求结束，但发生了网络错误
            emit self->sig_http_finish(id, "", ErrorCodes::ERR_NETWORK, mod);

            // 【注意】：这里你原代码写的是 self->deleteLater()，
            // 通常 reply 也是需要 deleteLater() 的，否则会内存泄漏。
            reply->deleteLater();
            return;
        }

        // 10. 读取服务器返回的所有原始数据，并转换为字符串格式
        QString res = reply->readAll();

        // 11. 发射自定义信号，通知 UI 层：请求成功，并带上返回的 JSON 结果
        emit self->sig_http_finish(id, res, ErrorCodes::SUCCESS, mod);

        // 12. 非常重要：通知 Qt 在稍后删除此 reply 对象以释放内存
        // 不能直接 delete reply，因为此时可能还有其他信号在处理中
        reply->deleteLater();

        return;
    });
}

void HttpMgr::slot_http_finish(const ReqId id,const QString &res, const ErrorCodes err,const Modules mod)
{
    //根据模块的类型发送指定的信号
    switch(mod)
    {
    case Modules::REGESTERMOD:  //通知注册模块http请求完成
        emit sig_reg_mod_finish(id, res, err);
        break;
    default:
        break;
    }
}
