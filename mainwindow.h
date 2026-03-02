#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QScreen>
#include<QStackedWidget>
#include "logindialog.h"
#include "registerdialog.h"
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
public slots:
    void slot_switch_reg();
    void slot_switch_login();

private:
    void set_mainwindow_minimum_size();
    Ui::MainWindow *ui;
    LoginDialog* login_dialog;
    RegisterDialog* register_dialog;
    QStackedWidget* stack_widget;

};
#endif // MAINWINDOW_H
