#include "mainwindow.h"

#include <QApplication>
#include "logindialog.h"
int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    LoginDialog login;
    if (login.exec() == QDialog::Accepted) {
        MainWindow w;
        w.setWindowIcon(QIcon(":/1.png")); // 使用资源中的图标
        //w.showFullScreen();  // 工业全屏显示，没有关闭图标
        w.showMaximized(); //铺满屏幕，但保留关闭最小化/logo图标
        w.show();
        return a.exec();
    } else {
        return 0; // 登录失败或取消，程序退出
    }
    return a.exec();
}
