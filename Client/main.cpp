#include "mainwindow.h"

#include <QApplication>
#include <QFile>
#include <QString>
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

    MainWindow w;
    w.show();

    return a.exec();
}
