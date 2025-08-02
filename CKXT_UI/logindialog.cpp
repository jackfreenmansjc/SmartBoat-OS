#include "logindialog.h"
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QMessageBox>
/*
LoginDialog::LoginDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("用户登录");
    setModal(true); // 模态窗口，阻止主界面操作

    auto layout = new QVBoxLayout(this);

    usernameEdit = new QLineEdit(this);
    passwordEdit = new QLineEdit(this);
    passwordEdit->setEchoMode(QLineEdit::Password);
    loginButton = new QPushButton("登录", this);

    layout->addWidget(new QLabel("用户名："));
    layout->addWidget(usernameEdit);
    layout->addWidget(new QLabel("密码："));
    layout->addWidget(passwordEdit);
    layout->addWidget(loginButton);

    connect(loginButton, &QPushButton::clicked, this, &LoginDialog::tryLogin);
}
*/
LoginDialog::LoginDialog(QWidget *parent)
    : QDialog(parent)
{
    // 设置窗口标题、图标、固定大小
    setWindowTitle("用户登录");
    setWindowIcon(QIcon(":/icons/icon.png")); // 你的 PNG 图标资源路径
    setModal(true);
    setFixedSize(300, 220); // 固定大小，避免变形

    // 设置样式（蓝白风格 + 圆角）
    this->setStyleSheet(R"(
        QWidget {
            background-color: #f0f8ff;
            font-family: 'Microsoft YaHei';
            font-size: 14px;
        }
        QLineEdit {
            padding: 6px;
            border: 1px solid #a0c4ff;
            border-radius: 6px;
            background: white;
        }
        QPushButton {
            background-color: #3a86ff;
            color: white;
            padding: 6px 12px;
            border-radius: 6px;
        }
        QPushButton:hover {
            background-color: #0077cc;
        }
        QLabel {
            margin-top: 8px;
            font-weight: bold;
        }
    )");

    // 输入框 & 按钮
    usernameEdit = new QLineEdit(this);
    usernameEdit->setPlaceholderText("请输入用户名");

    passwordEdit = new QLineEdit(this);
    passwordEdit->setPlaceholderText("请输入密码");
    passwordEdit->setEchoMode(QLineEdit::Password);

    loginButton = new QPushButton("登录", this);
    loginButton->setFixedWidth(100);

    // 布局
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(30, 20, 30, 20);
    mainLayout->setSpacing(10);

    mainLayout->addWidget(new QLabel("用户名："));
    mainLayout->addWidget(usernameEdit);
    mainLayout->addWidget(new QLabel("密码："));
    mainLayout->addWidget(passwordEdit);

    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnLayout->addStretch();
    btnLayout->addWidget(loginButton);
    btnLayout->addStretch();

    mainLayout->addLayout(btnLayout);

    // 连接槽
    connect(loginButton, &QPushButton::clicked, this, &LoginDialog::tryLogin);
}


void LoginDialog::tryLogin()
{
    QString username = usernameEdit->text();
    QString password = passwordEdit->text();

    // 多用户登录支持
    QMap<QString, QString> userMap = {
        {"me", "111111"},
        {"zhudaxian", "57880540sjc"},
        {"", ""}
    };

    if (userMap.contains(username) && userMap[username] == password) {
        accept();  // 登录成功，退出对话框
    } else {
        QMessageBox::warning(this, "错误", "用户名或密码错误！");
    }
}

QString LoginDialog::getUsername() const {
    return usernameEdit->text();
}
