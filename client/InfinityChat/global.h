#ifndef GLOBAL_H
#define GLOBAL_H

#include "QStyle"
#include <QWidget>
#include <functional>
#include <QRegularExpression>
#include <memory>
#include <iostream>
#include <mutex>
#include <QDebug>
#include <QByteArray>
#include <QNetworkReply>
#include <QJsonObject>
#include <QDir>
#include <QSettings>

/**
 * @brief repolish 用来刷新qss
 */
extern std::function<void(QWidget*)> repolish;

/**
 * @brief xorString 用于对注册密码的一个简单加密（http为明文传输）
 */
extern std::function<QString(QString)> xorString;

/**
 * @brief The ReqId enum 表示请求的ID
 */
enum ReqId {
    ID_GET_VARIFY_CODE = 1001,  //获取验证码
    ID_REG_USER = 1002,         //注册用户
};

/**
 * @brief The Modules enum 当前模式
 */
enum Modules {
    REGISTERMOD = 0,
};

/**
 * @brief The ErrorCodes enum 错误码
 */
enum ErrorCodes {
    SUCCESS = 0,
    ERR_JSON = 1,   //JSON 解析失败
    ERR_NETWORK = 2,    //网络错误
};

/**
 * @brief The TipErr enum 用户输入错误提示码
 */
enum TipErr{
    TIP_SUCCESS = 0,
    TIP_EMAIL_ERR = 1,
    TIP_PWD_ERR = 2,
    TIP_CONFIRM_ERR = 3,
    TIP_PWD_CONFIRM = 4,
    TIP_VARIFY_ERR = 5,
    TIP_USER_ERR = 6
};

/**
 * @brief The ClickLbState enum 自定义label状态
 */
enum ClickLbState{
    Normal = 0,
    Selected = 1
};

/**
 * @brief gate_url_prefix 网关url前缀
 */
extern QString gate_url_prefix;

#endif // GLOBAL_H
