#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDesktopWidget>
#include <QMessageBox>

/* led与蜂鸣器都属在led节点 */
#define LED_PATH                               "/sys/class/leds/sys-led"
#define BEEP_PATH                              "/sys/class/leds/beep"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    mainWindowInit();
}

void MainWindow::mainWindowInit()
{
    this->setGeometry(0,0,QApplication::desktop()->screenGeometry().width(),QApplication::desktop()->screenGeometry().height());
    //this->setGeometry(0,0,800,480);
    this->setWindowFlags(Qt::FramelessWindowHint); // 无边框

    mainWidget = new QWidget(this);
    mainWidget->installEventFilter(this);
    setCentralWidget(mainWidget);
    mainWidget->setStyleSheet("border-image:url(Resource/background/bg)");


    /* 循环动画面板Widget位置与大小确定 */
    for(int i = 0; i < 4; i++){
        loopWidgetArr[i] = new QWidget(mainWidget);
        switch (i) {
        case 0:
            loopWidgetArr[i]->setGeometry((float)550/800*this->geometry().width(),(float)150/480*this->geometry().height(),
                                          (float)180/800*this->geometry().width(),(float)120/480*this->geometry().height());
            break;
        case 1:
            loopWidgetArr[i]->setGeometry((float)200/800*this->geometry().width(),(float)100/480*this->geometry().height(),
                                          (float)300/800*this->geometry().width(),(float)200/480*this->geometry().height());
            break;
        case 2:
            loopWidgetArr[i]->setGeometry((float)550/800*this->geometry().width(),(float)275/480*this->geometry().height(),
                                          (float)180/800*this->geometry().width(),(float)120/480*this->geometry().height());
            break;
        case 3:
            loopWidgetArr[i]->setGeometry((float)200/800*this->geometry().width(),(float)305/480*this->geometry().height(),
                                          (float)300/800*this->geometry().width(),(float)200/480*this->geometry().height());
            break;
        default:
            break;
        }
        if (i < 2)
            loopWidgetArr[i]->installEventFilter(this);
    }

    loopWidgetArr[0]->setStyleSheet("QWidget{background-color:rgba(210,210,210,90%);"
                                    "border-radius: 5px;border-image:url(Resource/background/beepoff.png)}");
    loopWidgetArr[1]->setStyleSheet("QWidget{background-color:rgba(210,210,210,90%);"
                                    "border-radius: 5px;border-image:url(Resource/background/ledon.png)}");

    loopWidgetArr[3]->setStyleSheet("QWidget{background-color:rgba(210,210,210,40%);"
                                    "border-radius: 5px;border-image:url(Resource/background/ledshadowon.png)}");
    loopWidgetArr[2]->setStyleSheet("QWidget{background-color:rgba(210,210,210,40%);"
                                    "border-radius: 5px;border-image:url(Resource/background/beepshadowoff.png)}");

    for(int i=0; i<4; i++){
        /* 绑定动画对象 */
        propertyAnimation[i] = new QPropertyAnimation(loopWidgetArr[i], "geometry");
        propertyAnimation[i]->setDuration(500);//动画长度是0.5s
        /* 设置阴影效果，让循环面板不显生硬 */
        loopGraphicsDropShadowEffect[i] = new QGraphicsDropShadowEffect(this);
        loopGraphicsDropShadowEffect[i]->setOffset(-5,10);
        if (i < 2)
            loopGraphicsDropShadowEffect[i]->setColor(QColor("#444444"));
        else
            loopGraphicsDropShadowEffect[i]->setColor(QColor("#eeeeee"));

        loopGraphicsDropShadowEffect[i]->setBlurRadius(30);
        loopWidgetArr[i]->setGraphicsEffect(loopGraphicsDropShadowEffect[i]);
    }

}

/* 向前滑动 */
void MainWindow::forwardPushButtonClicked()
{
    if(propertyAnimation[1]->state() == QPropertyAnimation::Stopped){//等待动画完成才可以做下一次的动画
        for(int i = 0; i< 4; i++){
            propertyAnimation[i]->setStartValue(QRect(loopWidgetArr[i]->geometry().x(), loopWidgetArr[i]->geometry().y(),
                                                      loopWidgetArr[i]->geometry().width(), loopWidgetArr[i]->geometry().height())
                                                );
            loopWidgetArr[i]->setWindowFlags(Qt::WindowStaysOnTopHint);
            switch (i) {
            case 0:
                propertyAnimation[i]->setEndValue(QRect(loopWidgetArr[1]->geometry().x(), loopWidgetArr[1]->geometry().y(),
                        loopWidgetArr[1]->geometry().width(), loopWidgetArr[1]->geometry().height())
                        );
                break;
            case 1:
                propertyAnimation[i]->setEndValue(QRect(loopWidgetArr[0]->geometry().x(), loopWidgetArr[0]->geometry().y(),
                        loopWidgetArr[0]->geometry().width(), loopWidgetArr[0]->geometry().height())
                        );
                break;
            case 2:
                propertyAnimation[i]->setEndValue(QRect(loopWidgetArr[3]->geometry().x(), loopWidgetArr[3]->geometry().y(),
                        loopWidgetArr[3]->geometry().width(), loopWidgetArr[3]->geometry().height())
                        );
                break;
            case 3:
                propertyAnimation[i]->setEndValue(QRect(loopWidgetArr[2]->geometry().x(), loopWidgetArr[2]->geometry().y(),
                        loopWidgetArr[2]->geometry().width(), loopWidgetArr[2]->geometry().height())
                        );
                break;
            default:
                break;
            }
            propertyAnimation[i]->start();
        }
    }
}

/* 向后滑动 */
void MainWindow::backwardPushButtonClicked()
{
    if(propertyAnimation[1]->state() == QPropertyAnimation::Stopped){
        for(int i = 0; i< 4; i++){
            propertyAnimation[i]->setStartValue(QRect(loopWidgetArr[i]->geometry().x(), loopWidgetArr[i]->geometry().y(),
                                                      loopWidgetArr[i]->geometry().width(), loopWidgetArr[i]->geometry().height())
                                                );
            switch (i) {
            case 0:
                propertyAnimation[i]->setEndValue(QRect(loopWidgetArr[1]->geometry().x(), loopWidgetArr[1]->geometry().y(),
                        loopWidgetArr[1]->geometry().width(), loopWidgetArr[1]->geometry().height())
                        );
                break;
            case 1:
                propertyAnimation[i]->setEndValue(QRect(loopWidgetArr[0]->geometry().x(), loopWidgetArr[0]->geometry().y(),
                        loopWidgetArr[0]->geometry().width(), loopWidgetArr[0]->geometry().height())
                        );
                break;
            case 2:
                propertyAnimation[i]->setEndValue(QRect(loopWidgetArr[3]->geometry().x(), loopWidgetArr[3]->geometry().y(),
                        loopWidgetArr[3]->geometry().width(), loopWidgetArr[3]->geometry().height())
                        );
                break;
            case 3:
                propertyAnimation[i]->setEndValue(QRect(loopWidgetArr[2]->geometry().x(), loopWidgetArr[2]->geometry().y(),
                        loopWidgetArr[2]->geometry().width(), loopWidgetArr[2]->geometry().height())
                        );
            default:
                break;
            }
            propertyAnimation[i]->start();
        }
    }
}

/* 事件过滤器 */
bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{
    if(watched == mainWidget ){
        static QPoint point;
        switch (event->type()) {
        case QEvent::MouseButtonPress:
            point.setY(cursor().pos().y());     // 记录按下点的y坐标
            point.setX(cursor().pos().x());     // 记录按下点的x坐标
            break;
        case QEvent::MouseButtonRelease:
        {
            int offsety = cursor().pos().y() - point.y();
            int offsetx = cursor().pos().x() - point.x();

            if(qAbs(offsetx) > 50){ // 绝对值，求滑动的距离
                if(offsetx < 0)
                    backwardPushButtonClicked(); // 向后滚动
                else
                    forwardPushButtonClicked();  // 向前滚动
            }
            if(cursor().pos().y() > (float)150/480*this->geometry().height())//防止用户滑动大于150的区域而触发关闭按出现
                return true;
            if(offsety > 20){ // 判断如果这个值大于20，就出现关闭按钮
                static QWidget *widget = new QWidget(this);
                static QPushButton *pushButton = new QPushButton(widget);
                pushButton->setFocusPolicy(Qt::NoFocus);
                widget->setStyleSheet("QWidget{border-radius:6px;background-color:rgba(100,100,100,80%)}");
                widget->setGeometry(0,0,this->geometry().width(),40);
                pushButton->setGeometry(this->geometry().width()-40,0,40,40);
                pushButton->setStyleSheet("QPushButton{border-image:url(:/icon/Resource/icons/close.png);"
                                          "background-color:transparent}"
                                          "QPushButton:hover{border-image:url(:/icon/Resource/icons/close_hover.png);}"
                                          );
                connect(pushButton,SIGNAL(clicked()),this,SLOT(close()));
                widget->show();
                QEventLoop eventloop;
                QTimer::singleShot(3000, &eventloop, SLOT(quit())); // 延时3s后关闭
                eventloop.exec();
                widget->hide();
            }
        }

            break;
        default:
            break;
        }
    }
    for(int i = 0; i < 2; i++){
        if(watched == loopWidgetArr[i] ){
            static QPoint point;
            switch (event->type()) {
            case QEvent::MouseButtonPress:
                point.setY(cursor().pos().y());     // 记录按下点的y坐标
                point.setX(cursor().pos().x());     // 记录按下点的x坐标
                if((point.x()< (float)400/800*this->geometry().width()
                    && point.x() > (float)300/800*this->geometry().width()
                    && point.y() > (float)220/480*this->geometry().height()
                    && point.y() < (float)270/480*this->geometry().height())){
                    if (i == 1)
                        ledOnOffCtrol();
                    else
                        beepOnOffCtrol();
                }
                break;

                break;
            default:
                break;
            }
        }
    }

    return QWidget::eventFilter(watched,event);//将事件传递给父类
}


void MainWindow::ledOnOffCtrol()
{
    static bool flag = true;
    QString path = LED_PATH;
    static QString cmd_heartbeat = "echo heartbeat>"+ path +"/trigger";
    static QByteArray cmdby_heartbeat = cmd_heartbeat.toLatin1();
    static char* charCmd_heartbeat = cmdby_heartbeat.data();

    static QString cmd_off = "echo 0 >"+ path +"/brightness";
    static QByteArray cmdby_off = cmd_off.toLatin1();
    static char* charCmd_off = cmdby_off.data();
    if (flag) {
        system(charCmd_off);
        loopWidgetArr[3]->setStyleSheet("QWidget{background-color:rgba(210,210,210,25%);"
                                        "border-radius: 5px;border-image:url(Resource/background/ledshadowoff.png)}");
        loopWidgetArr[1]->setStyleSheet("QWidget{background-color:rgba(210,210,210,90%);"
                                        "border-radius: 5px;border-image:url(Resource/background/ledoff.png)}");
    } else{
        system(charCmd_heartbeat);
        loopWidgetArr[3]->setStyleSheet("QWidget{background-color:rgba(210,210,210,25%);"
                                        "border-radius: 5px;border-image:url(Resource/background/ledshadowon.png)}");
        loopWidgetArr[1]->setStyleSheet("QWidget{background-color:rgba(210,210,210,90%);"
                                        "border-radius: 5px;border-image:url(Resource/background/ledon.png)}");

    }
    flag = !flag;


}

void MainWindow::beepOnOffCtrol()
{
    static bool flag = false;
    static QString path = BEEP_PATH;
    static QString cmd_heartbeat = "echo heartbeat>"+ path +"/trigger";
    static QByteArray cmdby_heartbeat = cmd_heartbeat.toLatin1();
    static char* charCmd_heartbeat = cmdby_heartbeat.data();

    static QString cmd_off = "echo 0 >"+ path +"/brightness";
    static QByteArray cmdby_off = cmd_off.toLatin1();
    static char* charCmd_off = cmdby_off.data();

    if (flag) {
        system(charCmd_off);
        loopWidgetArr[2]->setStyleSheet("QWidget{background-color:rgba(210,210,210,40%);"
                                        "border-radius: 5px;border-image:url(Resource/background/beepshadowoff.png)}");
        loopWidgetArr[0]->setStyleSheet("QWidget{background-color:rgba(210,210,210,90%);"
                                        "border-radius: 5px;border-image:url(Resource/background/beepoff.png)}");
    } else{
        system(charCmd_heartbeat);
        loopWidgetArr[2]->setStyleSheet("QWidget{background-color:rgba(210,210,210,40%);"
                                        "border-radius: 5px;border-image:url(Resource/background/beepshadowon.png)}");
        loopWidgetArr[0]->setStyleSheet("QWidget{background-color:rgba(210,210,210,90%);"
                                        "border-radius: 5px;border-image:url(Resource/background/beepon.png)}");
    }
    flag = !flag;

}

MainWindow::~MainWindow()
{
    delete ui;

    for(int i = 0; i < 4; i++){
        delete propertyAnimation[i];
        delete loopGraphicsDropShadowEffect[i];
        delete loopWidgetArr[i];
    }
    delete mainWidget;
}
