#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>

namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = nullptr);
    ~LoginDialog();
signals:
    //切换注册窗口的信号
    void switch_register();
private:


    Ui::LoginDialog *ui;
};

#endif // LOGINDIALOG_H
