#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QHBoxLayout>
#include <QWidget>
#include <QToolBar>
#include <QToolButton>
#include <QMenu>
#include <QDialog>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QProgressBar>
#include <QFrame>
#include <QSpacerItem>
#include <QRandomGenerator>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // 定时器每秒更新时间
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::updateTitleWithTime);
    timer->start(1000); // 每 1000ms 刷新一次
    updateTitleWithTime(); // 初次更新标题

    setupTopStatusBar();//创建工具栏

    QTimer *timer_update_status = new QTimer(this);
    connect(timer_update_status, &QTimer::timeout, this, &MainWindow::updateStatusInfo);
    timer_update_status->start(1000);


}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::updateTitleWithTime()
{
    QString baseTitle = "喷水推进船舶智能测控验证平台";  // 你的主标题
    QString timeString = QDateTime::currentDateTime().toString("  yyyy年M月d日 HH:mm:ss");
    this->setWindowTitle(baseTitle + " - " + timeString);
}


void MainWindow::setupTopStatusBar()
{
    // 创建工具栏
    QToolBar *topBar = new QToolBar(this);
    topBar->setIconSize(QSize(64, 64)); // 设置图标大小
    topBar->setMovable(false);
    addToolBar(Qt::TopToolBarArea, topBar);

    // Logo 按钮
    QToolButton *logoButton = new QToolButton(this);
    logoButton->setIcon(QIcon(":/4logo.png"));  // 替换成你的图标路径
    logoButton->setIconSize(QSize(128, 128));
    logoButton->setToolTip("测控菜单");
    logoButton->setStyleSheet(R"(
    QToolButton {
        background-color: transparent;
        border: none;
        margin: 4px;
    }
    QToolButton:hover {
        background-color: #d0e7ff;
        border: 1px solid #4a90e2;
        border-radius: 6px;
    }
    )");

    topBar->addWidget(logoButton);

    // 弹出子菜单窗口（可复用）
    QDialog *popup = new QDialog(this, Qt::Popup);
    popup->setWindowFlags(Qt::Popup);
    popup->setStyleSheet("QDialog { background-color: #ffffff; border: 1px solid gray; }");
    popup->setFixedSize(220, 180);

    // 添加按钮
    QPushButton *btnConnect   = new QPushButton("连接 Connect", popup);
    QPushButton *btnTunning   = new QPushButton("整定 Tunning", popup);
    QPushButton *btnData      = new QPushButton("数据 Data", popup);
    QPushButton *btnSetting   = new QPushButton("设置 Setting", popup);
    QPushButton *btnAnalyzing = new QPushButton("分析 Analyzing", popup);

    for (auto btn : {btnConnect, btnTunning, btnData, btnSetting, btnAnalyzing}) {
        btn->setStyleSheet(R"(
        QPushButton {
            padding: 6px;
            background-color: #f0f0f0;
            border-radius: 4px;
        }
        QPushButton:hover {
            background-color: #4a90e2;
            color: white;
        }
    )");
    }

    QVBoxLayout *layout = new QVBoxLayout(popup);
    layout->setSpacing(6);
    layout->setContentsMargins(12, 12, 12, 12);
    layout->addWidget(btnConnect);
    layout->addWidget(btnTunning);
    layout->addWidget(btnData);
    layout->addWidget(btnSetting);
    layout->addWidget(btnAnalyzing);

    //
    connect(logoButton, &QToolButton::clicked, this, [=]() {
        QPoint pos = logoButton->mapToGlobal(QPoint(0, logoButton->height()));
        popup->move(pos);
        popup->show();
    });
    // 中间状态信息区域
    QWidget *statusWidget = new QWidget(this);
    QGridLayout *statusLayout = new QGridLayout(statusWidget);
    statusLayout->setContentsMargins(10, 0, 10, 0);
    statusLayout->setHorizontalSpacing(15);
    statusLayout->setVerticalSpacing(4);

    QStringList statusTexts = {
        "无线网关已连接", "移动站主机温度：40°C",
        "信号质量：66db", "室外气温：25°C",
        "东南风：2级 3.2m/s", "与移动站距离：350m",
        "GPS状态：良好", "网络延迟：20ms"
    };

    for (int i = 0; i < statusTexts.size(); ++i) {
        QLabel *label = new QLabel(statusTexts[i], this);
        label->setStyleSheet("color: black; background: transparent;");
        statusLayout->addWidget(label, i / 4, i % 4);
        statusLabels.append(label);  // 保存指针以供后续更新
    }
    topBar->addWidget(statusWidget);

    // 中间 spacer 用于将电池条推到右侧
    QWidget *spacer = new QWidget(this);
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    topBar->addWidget(spacer);

    // 右侧电量栏
    QWidget *batteryWidget = new QWidget(this);
    QVBoxLayout *batteryLayout = new QVBoxLayout(batteryWidget);
    batteryLayout->setContentsMargins(20, 4, 20, 4);// 左右预留空白区域
    batteryLayout->setSpacing(6);

    // battery bar 创建函数
    auto createBatteryBar=[&](const QString &labelText, int percent, int watt, int mins, QProgressBar **outBar, QProgressBar **outWattBar, QLabel **outInfoLabel) -> QWidget* {
        QWidget *barWidget = new QWidget(this);
        QHBoxLayout *barLayout = new QHBoxLayout(barWidget);
        barLayout->setContentsMargins(0, 0, 0, 0);
        barLayout->setSpacing(4);

        QLabel *label = new QLabel(labelText, this);
        label->setStyleSheet("color: black;");//min-width: 50px; text-align: right;");
        label->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

        QProgressBar *bar = new QProgressBar(this);
        bar->setRange(0, 100);
        bar->setValue(percent);
        bar->setFixedWidth(150); // 缩短电池条宽度
        //bar->setFixedHeight(6);
        QString color;
        if (percent > 60) color = "#007f00";        // 绿色
        else if (percent > 30) color = "#e6a300";   // 黄色
        else color = "#cc0000";                     // 红色
        bar->setStyleSheet(QString("QProgressBar::chunk { background: %1; } QProgressBar { border: 1px solid gray; background: transparent; height: 12px; }" ).arg(color));

        QProgressBar *wattBar = new QProgressBar(this);
        wattBar->setRange(0, 1000);
        wattBar->setValue(watt); // 假设最大功率 1000W
        wattBar->setFixedWidth(150); // 缩短电池条宽度
        wattBar->setStyleSheet("QProgressBar::chunk { background: cyan; } QProgressBar { border: none; background: transparent; height: 3px; }");
        wattBar->setFixedHeight(3);
        wattBar->setTextVisible(false);  // 不显示百分比

        QVBoxLayout *bars = new QVBoxLayout();
        bars->setSpacing(1);
        bars->addWidget(bar);
        bars->addWidget(wattBar);

        QLabel *info = new QLabel(QString("%1%, %2W, %3min").arg(percent).arg(watt).arg(mins), this);
        info->setStyleSheet("color: black;min-width: 100px;");

        barLayout->addWidget(label,0);
        barLayout->addLayout(bars,0);
        barLayout->addWidget(info,0);
        barLayout->addStretch(); // 推到最右侧

        // 返回指针
        if (outBar) *outBar = bar;
        if (outWattBar) *outWattBar = wattBar;
        if (outInfoLabel) *outInfoLabel = info;

        return barWidget;
    };


    batteryLayout->addWidget(createBatteryBar("24V电池", 58, 15, 125,&batteryBar24, &wattBar24, &batteryInfo24));
    batteryLayout->addWidget(createBatteryBar("48V电池", 76, 22, 215,&batteryBar48, &wattBar48, &batteryInfo48));
    batteryWidget->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
    topBar->addWidget(batteryWidget);
}

void MainWindow::updateStatusInfo()
{
    // 更新状态标签
    int temp = QRandomGenerator::global()->bounded(35, 55);
    int signal = QRandomGenerator::global()->bounded(50, 100);
    int delay = QRandomGenerator::global()->bounded(10, 50);
    int dist  = QRandomGenerator::global()->bounded(100, 500);

    QStringList dynamicTexts = {
        "无线网关已连接",
        QString("移动站主机温度：%1°C").arg(temp),
        QString("信号质量：%1db").arg(signal),
        "室外气温：26°C",
        "东南风：2级 3.5m/s",
        QString("与移动站距离：%1m").arg(dist),
        "GPS状态：良好",
        QString("网络延迟：%1ms").arg(delay)
    };

    for (int i = 0; i < statusLabels.size(); ++i) {
        statusLabels[i]->setText(dynamicTexts[i]);
    }

    // ===== 模拟 24V 电池 =====
    int percent24 = QRandomGenerator::global()->bounded(45, 90);
    int watt24    = QRandomGenerator::global()->bounded(10, 300);
    int mins24    = percent24 * 2;

    batteryBar24->setValue(percent24);
    wattBar24->setValue(watt24);

    QString color24 = (percent24 > 60) ? "#007f00" : (percent24 > 30 ? "#e6a300" : "#cc0000");
    batteryBar24->setStyleSheet(QString(
                                    "QProgressBar::chunk { background: %1; } QProgressBar { border: 1px solid gray; background: transparent; height: 12px; }"
                                    ).arg(color24));

    batteryInfo24->setText(QString("%1%, %2W, %3min").arg(percent24).arg(watt24).arg(mins24));

    // ===== 模拟 48V 电池 =====
    int percent48 = QRandomGenerator::global()->bounded(50, 95);
    int watt48    = QRandomGenerator::global()->bounded(10, 1000);
    int mins48    = percent48 * 2;

    batteryBar48->setValue(percent48);
    wattBar48->setValue(watt48);

    QString color48 = (percent48 > 60) ? "#007f00" : (percent48 > 30 ? "#e6a300" : "#cc0000");
    batteryBar48->setStyleSheet(QString(
                                    "QProgressBar::chunk { background: %1; } QProgressBar { border: 1px solid gray; background: transparent; height: 12px; }"
                                    ).arg(color48));

    batteryInfo48->setText(QString("%1%, %2W, %3min").arg(percent48).arg(watt48).arg(mins48));
}


