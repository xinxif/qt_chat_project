#include "logindialog.h"
#include "ui_logindialog.h"

LoginDialog::LoginDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
    connect(ui->register_btn,&QPushButton::clicked,this,&LoginDialog::switch_register);
}

LoginDialog::~LoginDialog()
{
    delete ui;
}
