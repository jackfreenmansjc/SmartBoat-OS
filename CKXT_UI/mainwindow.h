#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QTimer>
#include <QDateTime>
#include <QProgressBar>
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QTimer *timer;
    void updateTitleWithTime();
    void setupTopStatusBar();
    QVector<QLabel*> statusLabels;//系统和环境状态

    QProgressBar* batteryBar24 = nullptr;//电量显示条24v
    QProgressBar* wattBar24 = nullptr;//功率输出条24v电源功率

    QProgressBar* batteryBar48 = nullptr;//电量显示条48v
    QProgressBar* wattBar48 = nullptr;//功率输出条48v电源功率
    QLabel *batteryInfo24 = nullptr;
    QLabel *batteryInfo48 = nullptr;
    void updateStatusInfo();//状态刷新函数
};
#endif // MAINWINDOW_H

struct BatteryWidgets {
    QWidget* widget;

    QProgressBar* batteryBar;
    QProgressBar* wattBar;

};
