#include "mainwindow.h"

#include <QApplication>
#include <QFile>
#include <QString>
#include <QDir>
#include<QSettings>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QFile qss(":/style/stylesheet.qss");
    if(qss.open(QFile::ReadOnly))
    {
        qDebug()<<"open qss file success";
        QString style(qss.readAll());
        a.setStyleSheet(style);
        qss.close();
    }
    else
    {
        qDebug()<<"fail to open qss file";
    }
    // 获取当前应用程序的路径
    QString app_path(QCoreApplication::applicationDirPath());

    // 拼接文件名
    QString fileName = "config.ini";

    QString config_path = QDir::toNativeSeparators(app_path + QDir::separator() + fileName);
    QSettings settings(config_path, QSettings::IniFormat);

    //[GateServer] host=localhost port=8080
    QString gate_host = settings.value("GateServer/host").toString();
    QString gate_port = settings.value("GateServer/port").toString();

    gate_url_prefix = "http://"+gate_host+":"+gate_port;
    qDebug()<<gate_url_prefix<<"\n";



    MainWindow w;
    w.show();

    return a.exec();
}
