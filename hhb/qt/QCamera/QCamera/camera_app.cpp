#include "camera_app.h"
#include <QDebug>
#include <QFont>
#include <QImage>
#include <QMessageBox>
#include "informationdlg.h"
#include <QDesktopWidget>
#include <QApplication>

camera::camera(QWidget *parent):QDialog(parent),bufrgb(nullptr)
{
    //在写这个程序时用的屏幕分辨率是1024*600
    setFixedSize(QApplication::desktop()->geometry().width(),QApplication::desktop()->geometry().height());
    setAttribute(Qt::WA_DeleteOnClose,true); //关闭窗口释放内存
    QFont font;
    font.setPixelSize((float)18/600*this->geometry().height());

    //创建三个Frame
    midFrame = new QFrame(this);
    midFrame->setStyleSheet("QFrame{border-image:url(Resource/background/bg.png);}");
    midFrame->setGeometry(0,0,this->geometry().width(),this->geometry().height());

    topFrame = new QFrame(this);
    topFrame->setStyleSheet("QFrame{background:qlineargradient"
                            "(spread:pad,x1:0,y1:0,x2:1,y2:0,stop:0 rgba(169,169,169,50),stop:1 rgba(240,240,240,50));}");
    topFrame->setGeometry(0,0,this->geometry().width(),(float)48/600*this->geometry().height());

    topFrame->installEventFilter(this);

    bottomFrame = new QFrame(this);
    bottomFrame->setStyleSheet("QFrame{background:qlineargradient"
                               "(spread:pad,x1:0,y1:0,x2:1,y2:0,stop:0 rgba(240,240,240,0),stop:1 rgba(240,240,240,0));}");
    bottomFrame->setGeometry(0,(float)480/600*this->geometry().height(),this->geometry().width(),(float)120/600*this->geometry().height());

    //bottomFrame->setAttribute(Qt::WA_TranslucentBackground);//设置为透明颜色

    //摄像头选择标签
    cameraSelectLabel = new QLabel(topFrame);
    cameraSelectLabel->setGeometry(0,0,(float)120/1024*this->geometry().width(),(float)48/600*this->geometry().height());
    cameraSelectLabel->setFont(font);
    cameraSelectLabel->setText(tr("选择摄像头:"));
    cameraSelectLabel->setAttribute(Qt::WA_TranslucentBackground);//设置为透明颜色
    cameraSelectLabel->setStyleSheet("QLabel{color:white}");

    //摄像头下拉列表
    cameraSelect = new QComboBox(topFrame);
    cameraSelect->setGeometry((float)120/1024*this->geometry().width(),(float)8/600*this->geometry().height(),
                              (float)160/1024*this->geometry().width(),(float)35/600*this->geometry().height());
    cameraSelect->setFont(font);
    cameraSelect->setStyleSheet("QComboBox{background-color:#757575;}");
    //cameraSelect->setStyleSheet("QComboBox{background-color:rgba(224,244,244,70);}");
    cameraSelect->addItem("/dev/video1");
    cameraSelect->addItem("/dev/video2");
    connect(cameraSelect, SIGNAL(currentIndexChanged(const QString &)), this, SLOT(slotCameraSelect(const QString &)));

    //分辨率选择标签
    resolutionLabel = new QLabel(topFrame);
    resolutionLabel->setGeometry((float)300/1024*this->geometry().width(),0,(float)80/1024*this->geometry().width(),(float)48/600*this->geometry().height());
    resolutionLabel->setFont(font);
    resolutionLabel->setText(tr("分辨率:"));
    resolutionLabel->setAttribute(Qt::WA_TranslucentBackground);//设置为透明颜色
    resolutionLabel->setStyleSheet("QLabel{color:white}");

    //下拉列表
    resolutionSelect = new QComboBox(topFrame);
    resolutionSelect->setGeometry((float)380/1024*this->geometry().width(),(float)8/600*this->geometry().height(),
                                  (float)150/1024*this->geometry().width(),(float)35/600*this->geometry().height());
    resolutionSelect->setStyleSheet("QComboBox{background-color:#757575;}");
    resolutionSelect->setFont(font);
    resolutionSelect->addItem("640x480");
    resolutionSelect->addItem("1280x960");
    resolutionSelect->addItem("800x600");
    resolutionSelect->addItem("352x288");
    resolutionSelect->addItem("320x240");
    resolutionSelect->addItem("176x144");
    resolutionSelect->addItem("160x120");
    resolutionSelect->addItem("1280x720");
    connect(resolutionSelect, SIGNAL(currentIndexChanged(const QString &)), this, SLOT(slotResolutionSelect(const QString &)));

    //帧率显示label
    frameNameLabel = new QLabel(topFrame);
    frameNameLabel->setFont(font);
    frameNameLabel->setText(tr("帧率:"));
    frameNameLabel->setGeometry((float)560/1024*this->geometry().width(),0,(float)50/1024*this->geometry().width(),(float)48/600*this->geometry().height());
    frameNameLabel->setAttribute(Qt::WA_TranslucentBackground);//设置为透明颜色
    frameNameLabel->setStyleSheet("QLabel{color:white}");

    frameRateLabel = new QLabel(topFrame);
    frameRateLabel->setFont(font);
    frameRateLabel->setText(tr("0fps"));
    frameRateLabel->setStyleSheet("QLabel{color:#6495ED;}");
    frameRateLabel->setGeometry((float)610/1024*this->geometry().width(),0,(float)80/1024*this->geometry().width(),(float)48/600*this->geometry().height());
    frameRateLabel->setAttribute(Qt::WA_TranslucentBackground);//设置为透明颜色

    //控制按钮
    beginBtn = new QPushButton(bottomFrame);
    beginBtn->setGeometry((float)362/1024*this->geometry().width(),(float)30/600*this->geometry().height(),
                          (float)60/1024*this->geometry().width(),(float)60/1024*this->geometry().width());
    beginBtn->setText(tr("开始"));
    beginBtn->setStyleSheet("QPushButton{border-image:url(:/icon/Resource/icon/bl.png);"
                            "background-color:transparent;color:white;outline:none}");
    beginBtn->setFont(font);

    saveBtn = new QPushButton(bottomFrame);
    saveBtn->setGeometry((float)462/1024*this->geometry().width(),(float)10/600*this->geometry().height(),
                         (float)100/1024*this->geometry().width(),(float)100/1024*this->geometry().width());
    saveBtn->setText(tr("拍照"));
    saveBtn->setStyleSheet("QPushButton{border-image:url(:/icon/Resource/icon/saveBtn.png);"
                           "background-color:transparent;outline:none}");
    saveBtn->setFont(font);

    exitBtn = new QPushButton(bottomFrame);
    exitBtn->setGeometry((float)602/1024*this->geometry().width(),(float)30/600*this->geometry().height(),
                         (float)60/1024*this->geometry().width(),(float)60/1024*this->geometry().width());
    exitBtn->setText(tr("关闭"));
    exitBtn->setStyleSheet("QPushButton{border-image:url(:/icon/Resource/icon/bl.png);"
                           "background-color:transparent;color:white;outline:none}");
    exitBtn->setFont(font);

    connect(beginBtn, SIGNAL(clicked()), this, SLOT(slotBeginCapture()));
    connect(saveBtn, SIGNAL(clicked()), this, SLOT(slotSavePicture()));
    connect(exitBtn, SIGNAL(clicked()), this, SLOT(slotExitCamera()));

    displayLabel = new QLabel(midFrame);
    displayLabel->setMargin(0);
    displayLabel->setGeometry(0,0,this->geometry().width(),this->geometry().height());
    displayLabel->setAlignment(Qt::AlignCenter);  //水平居中对齐

    mainLayout = new QVBoxLayout(midFrame);
    mainLayout->setMargin(0);
    mainLayout->addWidget(displayLabel);

    timer =  new QTimer(this);
    timer->setInterval(10);
    connect(timer, SIGNAL(timeout()), this, SLOT(slotFlushBuff()));

    frameTimer = new QTimer(this);
    frameTimer->setInterval(1000);   //1S
    connect(frameTimer, SIGNAL(timeout()), this, SLOT(slotFrameRateTimer()));

    //默认分辨率
    imagewidth = 640;
    imageheight = 480;
    frameRate = 0;
    devname = "/dev/video1"; /* 默认摄像头为/dev/video1 */
    v4l2camera = nullptr;
    saveBtn->setDisabled(true);
    exitBtn->setDisabled(true);
    /* 暂时有个bug，单独运行这个程序没有问题，桌面调用运行时会闪退，所以禁用拍照功能 */
    saveBtn->hide();
}

int camera::slotBeginCapture()
{
    QByteArray name=devname.toLatin1();
    pd.dev_name = name.data();

    v4l2camera = new V4l2Camera;

    int flag;
    flag = v4l2camera->init_dev(&pd, imagewidth, imageheight);
    if(flag == -1){
        InformationDlg::information(this, tr("提醒"), tr("无相关设备，请检查!"));
        return flag;
    }
    else if(flag == -2)
    {
        InformationDlg::information(this, tr("提醒"), tr("设备打开错误,请检查!"));
        return flag;
    }
    else if(flag == -3)
    {
        InformationDlg::information(this, tr("提醒"), tr("不能打开设备，请检查！"));
        return flag;
    }
    timer->start();
    frameTimer->start();

    beginBtn->setDisabled(true);
    cameraSelect->setDisabled(true);
    resolutionSelect->setDisabled(true);

    saveBtn->setEnabled(true);
    exitBtn->setEnabled(true);
}

int camera::slotSavePicture()
{
    FILE *fp;
    time_t now;
    struct tm *tm_time;
    time(&now);

    if(v4l2camera == nullptr)
        return -1;

    if(bufrgb > 0 && strlen((char *)bufrgb) > 0)
    {
        tm_time = localtime(&now);
        char filename[30] = {0};
        sprintf(filename,"/home/root/%4d-%02d-%02d_%02d.%02d.%02d.png",1900+tm_time->tm_year,1+tm_time->tm_mon,tm_time->tm_mday,
                      tm_time->tm_hour,tm_time->tm_min,tm_time->tm_sec);
        QImage image(bufrgb, imagewidth, imageheight, QImage::Format_RGB888);
        image.save(filename);
    }
}

void camera::slotExitCamera()
{
    if(v4l2camera != nullptr)
    {
        timer->stop();
        frameTimer->stop();
        frameRateLabel->setText(tr("0fps"));
        v4l2camera->stop_dev(&pd);
        if(bufrgb!=nullptr)
        {
            free(bufrgb);   //释放内存
            bufrgb=nullptr;
        }
        delete v4l2camera;
        v4l2camera=nullptr;
    }

    beginBtn->setEnabled(true);
    saveBtn->setDisabled(true);
    exitBtn->setDisabled(true);
    cameraSelect->setEnabled(true);
    resolutionSelect->setEnabled(true);
}

void camera::slotCameraSelect(const QString &item)
{
    devname=item;
}

void camera::slotResolutionSelect(const QString &item)
{
    int index = 0;
    index = resolutionSelect->currentIndex();
    switch (index) {
        case 0:
            imagewidth = 640;
            imageheight = 480;
            break;
        case 1:
            imagewidth = 1280;
            imageheight = 960;
            break;
        case 2:
            imagewidth = 800;
            imageheight = 600;
            break;
        case 3:
            imagewidth = 352;
            imageheight = 288;
            break;
        case 4:
            imagewidth = 320;
            imageheight = 240;
            break;
        case 5:
            imagewidth = 176;
            imageheight = 144;
            break;
        case 6:
            imagewidth = 160;
            imageheight = 120;
            break;
        case 7:
            imagewidth = 1280;
            imageheight = 720;
            break;
    }
}

//帧率统计
void camera::slotFrameRateTimer()
{
    QString frame;

    frame = QString::number(frameRate) + "fps";
    frameRateLabel->setText(frame);
    //qDebug()<<"fame rate="<<frameRate;
    frameRate=0;
}

int camera::slotFlushBuff()
{
    int ret = 0;
    if(v4l2camera == nullptr)
        return -1;

    if(v4l2camera->read_frame(&pd) == -1) //读取数据帧
    {
        slotExitCamera();
        return -2;
    }

    if(!bufrgb)
    {
        bufrgb = (unsigned char *)malloc(imagewidth * imageheight *3);
    }
    memset(bufrgb, 0, imagewidth * imageheight * 3); //清零
    v4l2camera->convert_yuv_to_rgb_buffer((unsigned char *)pd.buffers[pd.buf.index].start,
                                        bufrgb,
                                        imagewidth,
                                        imageheight);
    QImage image(bufrgb, imagewidth,imageheight,QImage::Format_RGB888);
    displayLabel->setPixmap(QPixmap::fromImage(image));
    frameRate++;    //帧率加1
    ret = v4l2camera->return_data(&pd);
    if(ret < 0)
    {
        slotExitCamera();
        return -3;
    }
}

void camera::closeApp()
{
    if(v4l2camera != nullptr)
    {
        timer->stop();
        frameTimer->stop();
        frameRateLabel->setText(tr("0fps"));
        v4l2camera->stop_dev(&pd);
        if(bufrgb!=nullptr)
        {
            free(bufrgb);   //释放内存
            bufrgb=nullptr;
        }
        delete v4l2camera;
        v4l2camera=nullptr;
    }
    close();    //关闭窗口
}

camera::~camera()
{

}
bool camera::eventFilter(QObject *watched, QEvent *event)
{
    static QPoint point;
    /* 定义事件过滤器，指定在主窗口上检测 */
    if(watched == topFrame){
        switch (event->type()) {
        case QEvent::MouseButtonPress:
            point.setY(cursor().pos().y());     // 记录按下点的y坐标
            break;
        case QEvent::MouseButtonRelease:
        {
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
                connect(pushButton,SIGNAL(clicked()),this,SLOT(closeApp()));
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
    return QWidget::eventFilter(watched,event);//将事件传递给父类
}
