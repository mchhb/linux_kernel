#include "mainwindow.h"
//事件过滤器单独一个cpp文件，因为太长
bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{
    /* 主窗体事件 */
    if(watched == this){
        static QPoint point;
        switch (event->type()) {
        case QEvent::MouseButtonPress:
            recorderListWidget->setCurrentRow(-2);          // 设置录音列表当前项失去焦点
            deletePushButton->hide();
            point.setY(cursor().pos().y());                 // 记录按下点的y坐标
            break;
        case QEvent::MouseButtonRelease:
        {
            QPoint pt = cursor().pos();
            if(pt.y() > (float)150/480*this->geometry().height())     //防止用户滑动进度条而触发关闭按钮出现
                return true;
            int offset = cursor().pos().y() - point.y();
            if(offset > 30){ // 判断如果这个值大于30，就出现关闭按钮
                static QWidget *widget = new QWidget(this);
                static QPushButton *pushButton = new QPushButton(widget);
                pushButton->setFocusPolicy(Qt::NoFocus);
                widget->setStyleSheet("QWidget{border-radius:6px;background-color:rgba(100,100,100,80%);border-image:transparent}");
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

    /* 媒体播放进度条事件，点击播放进度条时，改变媒体的播放位置 */
    if(watched == mediaSlider){
        switch (event->type()) {
        case QEvent::MouseButtonPress:
            mediaSliderPressFlag = true;
            if(mediaSliderPressFlag){
                QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
                float posPosition;
                posPosition = (float)mouseEvent->pos().x()/mediaSlider->geometry().width();
                mediaSlider->setValue(posPosition*mediaSlider->maximum());
                mediaplayer->setPosition(mediaSlider->value() * 1000);
            }
            break;

        case QEvent::MouseButtonRelease:
            mediaSliderPressFlag = false;
            break;

        default:
            break;
        }
    }

    /* 静音按钮事件 */
    if(watched == mutePushButton){
        switch (event->type()) {
        case QEvent::Enter:
            volumeslider->setVisible(true);
            timerVolumeSlider->stop();
            break;

        case QEvent::MouseButtonPress:
            break;

        case QEvent::Leave:
            timerVolumeSlider->start(1000);
            break;
        default:
            break;
        }
    }

    /* 监测音量条事件 */
    if(watched == volumeslider){
        switch (event->type()) {
        case QEvent::Leave:
            volumeslider->setVisible(false);                //设置音量条不可见
            break;

        case QEvent::Enter:
            timerVolumeSlider->stop();                      //停止音量条定时器
            break;

        case QEvent::MouseButtonPress:
            /*  点击音量条的位置设置音量到该位置 */
            if(1){
                QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
                float posPosition;
                posPosition = (float)mouseEvent->pos().y()/volumeslider->geometry().height();
                volumeslider->setValue((1 - posPosition)*volumeslider->maximum());
                mediaplayer->setVolume(volumeslider->value());
            }
            break;

        default:
            break;
        }
    }
    return QWidget::eventFilter(watched,event);//将事件传递给父类
}
