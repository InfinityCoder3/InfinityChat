#include "logindialog.h"
#include "ui_logindialog.h"
#include <QDebug>

LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog)
{
    ui->setupUi(this);

    // 提示框
    ui->email_edit->setPlaceholderText("请输入邮箱号");
    ui->pass_edit->setPlaceholderText("请输入登录密码");

    connect(ui->reg_btn, &QPushButton::clicked, this, &LoginDialog::sigSwitchRegister);
}

LoginDialog::~LoginDialog()
{
    qDebug() << __FUNCTION__;
    delete ui;
}
