#include "registerdialog.h"
#include "global.h"
#include "ui_registerdialog.h"

RegisterDialog::RegisterDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::RegisterDialog)
{
    ui->setupUi(this);

    //Qt 在运行时修改动态属性后： 样式不会自动刷新，需要手动刷新
    //不设置 state，这个 QLabel 根本没有 state 这个属性
    ui->tip_lable->setProperty("state","normal");
    repolish(ui->tip_lable);
    connect(ui->cancel_btn,&QPushButton::clicked,this,&RegisterDialog::swtich_login);
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
    }
    else
    {
        //提示邮箱不正确
        show_tip(tr("邮箱地址不正确"),false);
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

