#include <mainwindow.h>
#include <QEventLoop>
//事件过滤器单独一个cpp文件，因为太长
bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{
    static QPoint point;
    if(watched == this){                                    //定义事件过滤器，指定在主窗口上检测
        switch (event->type()) {
        case QEvent::MouseButtonPress:
            if(playBackModeList->isVisible()){
                playBackModeList->setVisible(false);
            }
            point.setY(cursor().pos().y());     // 记录按下点的y坐标
            break;

        case QEvent::MouseButtonRelease:
        {
            QPoint pt = cursor().pos();
            if(pt.y() > (float)150/480*this->geometry().height())//防止用户长按进度条而触发关闭按出现
                return true;
            int offset = cursor().pos().y() - point.y();
            if(offset > 20){ // 判断如果这个值大于20，就出现关闭按钮
                static QWidget *widget = new QWidget(this);
                static QPushButton *pushButton = new QPushButton(widget);
                pushButton->setFocusPolicy(Qt::NoFocus);
                widget->setStyleSheet("QWidget{border-radius:6px;background-color:rgba(100,100,100,80%)}");
                widget->setGeometry(0,0,this->geometry().width(),40);
                pushButton->setGeometry(this->geometry().width()-40,0,40,40);
                pushButton->setStyleSheet("QPushButton{border-image:url(:/icon/Resource/icon/close.png);"
                                          "background-color:transparent}"
                                          "QPushButton:hover{border-image:url(:/icon/Resource/icon/close_hover.png);}"
                                          );
                connect(pushButton,SIGNAL(clicked()),this,SLOT(close()));
                widget->show();
                QEventLoop eventloop;
                QTimer::singleShot(3000, &eventloop, SLOT(quit())); // 延时3s后隐藏
                eventloop.exec();
                widget->hide();
            }
        }
            break;
        default:
            break;
        }
    }

    if(watched == playBackModeList){                         //定义事件过滤器，指定在播放模式列表上检测
        switch (event->type()) {
        case QEvent::Leave:                                  //离开时设置当前选择的项为高亮，其他设置为原来的
            playBackModeList->currentItem()->setForeground(QColor(255, 255, 255));
            playBackModeList->currentItem()->setIcon(QIcon(pbModeArr[playBackModeList->currentRow()].icon2));;
            for (int i = 0; i < 5; i++) {
                if(i != playBackModeList->currentRow() )
                    playBackModeList->item(i)->setIcon(QIcon(pbModeArr[i].icon1));
            }
            break;
        default:
            break;
        }
    }

    if(watched == mutePushButton){                            //定义事件过滤器，指定在静音按钮上检测
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

    if(watched == volumeslider){                            //定义事件过滤器，指定在音量条上检测
        switch (event->type()) {
        case QEvent::Leave:
            volumeslider->setVisible(false);                //设置音量条不可见
            break;

        case QEvent::Enter:
            timerVolumeSlider->stop();                      //停止音量条定时器
            break;

        case QEvent::MouseButtonPress:
        {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
            float posPosition;
            posPosition = (float)mouseEvent->pos().y()/volumeslider->geometry().height();
            volumeslider->setValue((1 - posPosition)*volumeslider->maximum());
            mediaPlayer->setVolume(volumeslider->value());
        }
            break;

        default:
            break;
        }
    }

    if(watched == songsPlaySlider){                         //定义事件过滤器，指定在音量条上检测
        switch (event->type()) {
        case QEvent::MouseButtonPress:
            songsPlaySliderPressFlag = true;
            if(songsPlaySliderPressFlag){
                QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
                float posPosition;
                posPosition = (float)mouseEvent->pos().x()/songsPlaySlider->geometry().width();
                songsPlaySlider->setValue(posPosition*songsPlaySlider->maximum());
                mediaPlayer->setPosition(songsPlaySlider->value() * 1000);
            }
            break;

        case QEvent::MouseButtonRelease:
            songsPlaySliderPressFlag = false;
            break;

        default:
            break;
        }
    }

    if(watched == settingsList){                            //定义事件过滤器，指定在设置列表上检测
        switch (event->type()) {
        case QEvent::Leave:
            settingsList->setCurrentRow(-1);
            break;

        default:
            break;
        }
    }

    return QWidget::eventFilter(watched,event);//将事件传递给父类
}
