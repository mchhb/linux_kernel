#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "weater.h"
#include <QMainWindow>
#include <QHBoxLayout>
#include <QLabel>
#include <QDateTime>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonArray>
#include <QTextCodec>
#include <QFileInfo>

#include <QGraphicsDropShadowEffect>
#include <QPropertyAnimation>
#include <QTimer>
#include <QPushButton>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void mainWindowInit();                          // 主窗体布局初始化

private:
    Ui::MainWindow *ui;

    QWidget         *mainWidget;                    // 主Widget
    QWidget         *loopWidgetArr[4];              // 循环动画面板arr

    QGraphicsDropShadowEffect *loopGraphicsDropShadowEffect[4];
    QPropertyAnimation *propertyAnimation[4];

    QPushButton     *ledButton;                     // 控制Led的按钮
    QPushButton     *beepButton;                    // 控制beep的按钮
    QLabel          *ledLabel;                      // 控制Led的按钮
    QLabel          *beepLabel;                     // 控制beep的按钮
    QVBoxLayout     *vlayout1;                      // 布局1
    QVBoxLayout     *vlayout2;
    bool eventFilter(QObject *watched, QEvent *event);
    void ledOnOffCtrol();
    void beepOnOffCtrol();

private slots:
    void backwardPushButtonClicked();
    void forwardPushButtonClicked();

};

#endif // MAINWINDOW_H
