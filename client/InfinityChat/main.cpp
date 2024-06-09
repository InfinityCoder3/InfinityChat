#include "mainwindow.h"

#include <QApplication>
#include <QFile>
#include "global.h"
#include <QSettings>
#include <QDir>

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

    // 获取当前程序路径
    QString app_path = QCoreApplication::applicationDirPath();
    QString fileName = "config.ini";
    QString config_path = QDir::toNativeSeparators(app_path + QDir::separator() +
                                                   "config" + QDir::separator() + fileName);
    // 解析ini文件，获取host&port
    QSettings s(config_path, QSettings::IniFormat);
    QString gate_host = s.value("GateServer/host").toString();
    QString gate_port = s.value("GateServer/port").toString();
    gate_url_prefix = "http://" + gate_host + ":" + gate_port;
    qDebug() << "gate_rul_prefix: " << gate_url_prefix;


    MainWindow w;
    w.show();
    return a.exec();
}
