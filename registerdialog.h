#ifndef REGISTERDIALOG_H
#define REGISTERDIALOG_H

#include <QDialog>
#include <QRegularExpression>
#include "global.h"
namespace Ui {
class RegisterDialog;
}

class RegisterDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RegisterDialog(QWidget *parent = nullptr);
    ~RegisterDialog();
signals:
    void swtich_login();

private slots:
    void on_get_verify_btn_clicked();
    void slot_reg_mod_finish(const ReqId id,const  QString &res,const ErrorCodes err);
private:
    //注册窗口的提示信息
    void show_tip(const QString&msg,const bool ok);
    void init_http_handlers();
    Ui::RegisterDialog *ui;
    QMap<ReqId,std::function<void(const QJsonObject&)>> handlers;
};

#endif // REGISTERDIALOG_H
