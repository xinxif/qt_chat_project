#include "registerdialog.h"
#include "global.h"
#include "ui_registerdialog.h"
#include"httpmgr.h"
RegisterDialog::RegisterDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::RegisterDialog)
    ,handlers()
{
    ui->setupUi(this);

    //Qt 在运行时修改动态属性后： 样式不会自动刷新，需要手动刷新
    //不设置 state，这个 QLabel 根本没有 state 这个属性
    ui->tip_lable->setProperty("state","normal");
    repolish(ui->tip_lable);
    connect(ui->cancel_btn,&QPushButton::clicked,this,&RegisterDialog::swtich_login);

    //初始化HTTP处理器
    init_http_handlers();
    
    //
    connect(HttpMgr::get_instance().get(),&HttpMgr::sig_reg_mod_finish,this,&RegisterDialog::slot_reg_mod_finish);
}

RegisterDialog::~RegisterDialog()
{
    delete ui;
}

void RegisterDialog::on_get_verify_btn_clicked()
{
    auto email = ui->email_edit->text();
    QRegularExpression regx(R"(^(\w+)@(\w+)(\.(\w+))+$)");
    bool match = regx.match(email).hasMatch();
    if(match)
    {
        //发送http请求获取验证码
        QJsonObject json_obj;
        json_obj["email"]=email;
        HttpMgr::get_instance()->post_http_req(QUrl(gate_url_prefix+"/get_varifycode"),json_obj,ReqId::ID_GET_VERIFY_CODE,Modules::REGESTERMOD);
    }
    else
    {
        //提示邮箱不正确
        show_tip(tr("邮箱地址不正确"),false);
    }
}

void RegisterDialog::slot_reg_mod_finish(const ReqId id,const  QString &res,const ErrorCodes err)
{
    if(err!=ErrorCodes::SUCCESS)
    {
        this->show_tip(tr("网络请求错误"),false);
        return;
    }
    // 解析 JSON 字符串,res需转化为QByteArray
    QJsonDocument jsonDoc = QJsonDocument::fromJson(res.toUtf8());
    //json解析错误
    if(jsonDoc.isNull()){
        show_tip(tr("json解析错误"),false);
        return;
    }

    //json解析错误
    if(!jsonDoc.isObject()){
        show_tip(tr("json解析错误"),false);
        return;
    }

    QJsonObject jsonObj = jsonDoc.object();

    //检查处理器是否存在，防止调用空函数
    if(handlers.contains(id))
    {
        handlers[id](jsonObj);
    }
    else
    {
        qWarning() << "No handler found for request id:" << static_cast<int>(id);
    }
}

void RegisterDialog::show_tip(const QString &msg, const bool ok)
{

    if(ok)
    {
        ui->tip_lable->setProperty("state","normal");

    }
    else
    {
        ui->tip_lable->setProperty("state","err");
    }
    ui->tip_lable->setText(msg);
    repolish(ui->tip_lable);
}

void RegisterDialog::init_http_handlers()
{
    handlers.insert(ReqId::ID_GET_VERIFY_CODE,[this](const QJsonObject &json_obj)
    {
        int error = json_obj["error"].toInt();
        if(error!=static_cast<int>(ErrorCodes::SUCCESS))
        {
            show_tip(tr("参数错误"),false);
            return;
        }

        auto email = json_obj["email"].toString();
        show_tip(tr("验证码已发送到邮箱，注意查收"),true);
        qDebug()<< "email is " << email ;
    });
}

