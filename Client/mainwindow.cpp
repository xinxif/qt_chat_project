#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , login_dialog(new LoginDialog),register_dialog(new RegisterDialog)
    , stack_widget(new QStackedWidget)
{
    ui->setupUi(this);
    this->set_mainwindow_minimum_size();

    //拆用stackedwidget切换多个页面
    this->setCentralWidget(stack_widget);
    this->stack_widget->addWidget(this->login_dialog);
    this->stack_widget->addWidget(this->register_dialog);
    this->stack_widget->setCurrentWidget(this->login_dialog);

    connect(login_dialog,&LoginDialog::switch_register,this,&MainWindow::slot_switch_reg);
    connect(register_dialog,&RegisterDialog::swtich_login,this,&MainWindow::slot_switch_login);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::slot_switch_reg()
{
    // this->login_dialog->hide();
    // this->setCentralWidget(this->register_dialog);
    // this->register_dialog->show();
    // setCentralWidget会删除旧的widget，this->register_dialog会被delete
    this->stack_widget->setCurrentWidget(this->register_dialog);
}

void MainWindow::slot_switch_login()
{
    // this->register_dialog->hide();
    // this->setCentralWidget(this->login_dialog);
    // this->login_dialog->show();
    this->stack_widget->setCurrentWidget(this->login_dialog);
}

void MainWindow::set_mainwindow_minimum_size()
{
    //获取主屏幕对象
    QScreen *screen = QGuiApplication::primaryScreen();

    // 获取屏幕的可用区域（排除任务栏/Dock栏）
    // 如果想获取包含任务栏的全屏大小，可以使用 screen->geometry()
    QRect screenGeometry = screen->availableGeometry();
    int screenWidth = screenGeometry.width();
    int screenHeight = screenGeometry.height();

    // 计算窗口大小
    // 假设我们希望窗口高度占据屏幕高度的 50% (作为一个合理的最小值)
    int minHeight = static_cast<int>(screenHeight * 0.43);
    int minWidth = static_cast<int>(minHeight / 1.4);

    // 如果计算出的宽度超过了屏幕宽度（极少数情况），则以宽度为基准反推
    if (minWidth > screenWidth) {
        minWidth = static_cast<int>(screenWidth * 0.43);
        minHeight = static_cast<int>(minWidth * 1.4);
    }

    //设置最小尺寸
    this->setMinimumSize(minWidth, minHeight);

    //让窗口启动时直接就是这个大小
    this->resize(minWidth, minHeight);
}
