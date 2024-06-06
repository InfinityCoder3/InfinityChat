#include "mainwindow.h"

#include <QApplication>
#include <QFile>
#include "global.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // 加载qss样式表
    QFile qss(":/style/stylesheet.qss");
    if (qss.open(QFile::ReadOnly)) {
        QString style = qss.readAll();
        a.setStyleSheet(style);
        qDebug() << "qss file open succussed!";
        qss.close();
    } else {
        qDebug() << "qss file open failed!";
    }

    MainWindow w;
    w.show();
    return a.exec();
}
