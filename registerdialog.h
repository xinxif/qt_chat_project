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

private:
    void show_tip(const QString&msg,const bool ok);

    Ui::RegisterDialog *ui;
};

#endif // REGISTERDIALOG_H
