#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // 相关初始化设置
    this->setWindowTitle("InfinityChat");
    this->setFixedSize(300,500);
    this->setWindowIcon(QIcon(":/img/infinity.png"));

    // 创建一个CentralWidget, 并将其设置为MainWindow的中心部件
    _login_dlg = new LoginDialog(this);
    // 设置无边框属性
    _login_dlg->setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);
    setCentralWidget(_login_dlg);

    //连接登录界面注册信号
    connect(_login_dlg, &LoginDialog::sigSwitchRegister, this, &MainWindow::slotSwitchReg);


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::slotSwitchReg()
{
    _reg_dlg = new RegisterDialog(this);
    //    _reg_dlg->hide();
    _reg_dlg->setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);

    //连接注册界面返回登录信号
    connect(_reg_dlg, &RegisterDialog::sigSwitchLogin, this, &MainWindow::slotSwitchLogin);
    setCentralWidget(_reg_dlg);
    //   _login_dlg->hide();
    _reg_dlg->show();
}

void MainWindow::slotSwitchLogin()
{
    //创建一个CentralWidget, 并将其设置为MainWindow的中心部件
    _login_dlg = new LoginDialog(this);
    _login_dlg->setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);
    setCentralWidget(_login_dlg);

//    _reg_dlg->hide();
//    _login_dlg->show();
    //连接登录界面注册信号
    connect(_login_dlg, &LoginDialog::sigSwitchRegister, this, &MainWindow::slotSwitchReg);
}

