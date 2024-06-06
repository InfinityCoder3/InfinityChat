#ifndef HTTPMGR_H
#define HTTPMGR_H

#include "singleton.h"
#include <QString>
#include <QUrl>
#include <QObject>
#include <QNetworkAccessManager>
#include <QJsonObject>
#include <QJsonDocument>

/*
 * CRTP（Curiously Recurring Template Pattern），中文称为“奇异递归模板模式”，
 * 是一种在 C++ 中使用模板实现静态多态性的技巧。
 * 这种模式使得派生类可以在编译时提供自己的特化行为，而不需要运行时的虚函数调用开销。
 * CRTP 的典型实现形式是派生类将自身作为模板参数传递给基类。
*/

class HttpMgr : public QObject, public Singleton<HttpMgr>,
        public std::enable_shared_from_this<HttpMgr>
{
    Q_OBJECT
public:
    ~HttpMgr();
    void PostHttpReq(QUrl url, QJsonObject json, ReqId req_id, Modules mod);

private:
    friend class Singleton<HttpMgr>;
    HttpMgr();


private:
    QNetworkAccessManager _manager;

private slots:
    void slot_http_finish(ReqId id, QString res, ErrorCodes err, Modules mod);

signals:
    void sig_http_finish(ReqId id, QString res, ErrorCodes err, Modules mod);
    void sig_reg_mod_finish(ReqId id, QString res, ErrorCodes err);
};

#endif // HTTPMGR_H
