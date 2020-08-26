#include <QtGui>
#include "config.h"
#include "processImage.h"
#include "videodevice.h"
#include <QMessageBox>
#include <QHBoxLayout>
#include <Qt>
#include <QDesktopWidget>
#include <QApplication>
#include <QEventLoop>
#include <stdio.h>
extern "C"
{
#include <stdio.h>
#include <stdlib.h>
}
ProcessImage::ProcessImage(QWidget *parent):QWidget(parent)
{
    flag = true;
    /* 设置程序界面大小为屏幕大小 */
    this->setGeometry(0,0,QApplication::desktop()->width(),QApplication::desktop()->height());
    this->setStyleSheet("background-color:rgb(255,255,255)");
    /* 可对应节点修改，这里默认是/dev/video1节点，使用板上接口摄像头时请不要接usb摄像头，否则节点可能不是video1 */
    vd = new VideoDevice(tr("/dev/video1"));

    connect(vd, SIGNAL(display_error(QString)), this,SLOT(display_error(QString)));
    // 判断是否有此节点，没有此节点就直接退出程序了
    rs = vd->open_device();
    if(-1 == rs)
    {
        //QMessageBox::warning(this,tr("error"),tr("open /dev/dsp error"),QMessageBox::Yes);
        QMessageBox::warning(this,tr("error"),tr("打开摄像头设备节点失败!"),QMessageBox::Yes);
        vd->close_device();
        flag = false;
        this->close(); // 打开摄像头节点失败，关闭程序
    }
    //label->setGeometry(0,0,320,240);
    rs = vd->init_device();
    if(-1==rs)
    {
        // QMessageBox::warning(this,tr("error"),tr("init failed"),QMessageBox::Yes);
        QMessageBox::warning(this,tr("error"),tr("初始化摄像头失败!"),QMessageBox::Yes);
        vd->close_device();
        flag = false;
        this->close(); // 初始化失败后，关闭程序
    }
    if (!flag)  // 如果初始化摄像头失败，下面的就不用做了
        return;

    //分配内存
    pp = (unsigned char *)malloc(CAPTURE_WIDTH * CAPTURE_HIGHT/*QWidget::width()*QWidget::height()*/* 3 * sizeof(char));

    //设置图像格式
    frame = new QImage(pp,CAPTURE_WIDTH,CAPTURE_HIGHT,QImage::Format_RGB888);

    label = new QLabel(this);
    label->setScaledContents(true);//自动适应屏幕

    //开始捕获
    rs = vd->start_capturing();
    if(-1==rs)
    {
        QMessageBox::warning(this,tr("error"),tr("start capture failed"),QMessageBox::Yes);
        vd->close_device();
    }
    if(-1==rs)
    {
        QMessageBox::warning(this,tr("error"),tr("get frame failed"),QMessageBox::Yes);
        vd->stop_capturing();
    }

    flag = true;

    //开启一个定时器，用来刷新界面，30ms刷新一次
    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(update()));
    timer->start(30);

    QHBoxLayout *hLayout = new QHBoxLayout();
    hLayout->addWidget(label);
    setLayout(hLayout);
    //setWindowTitle(tr("Capture"));
    this->installEventFilter(this);//安装事件过滤器

}

ProcessImage::~ProcessImage()
{

    delete label;
    if(!flag){
        delete timer;
        rs = vd->stop_capturing();
        rs = vd->uninit_device();
        rs = vd->close_device();
    }
}

void ProcessImage::paintEvent(QPaintEvent *)
{
    rs = vd->get_frame((void **)&p,&len);
    convert_yuv_to_rgb_buffer(p,pp,CAPTURE_WIDTH,CAPTURE_HIGHT/*QWidget::width(),QWidget::height()*/);
    //frame->loadFromData((uchar *)p,/*len*/320 * 240 * 3 * sizeof(char));

    label->setPixmap(QPixmap::fromImage(*frame,Qt::AutoColor));
    rs = vd->unget_frame();

}

void ProcessImage::display_error(QString err)
{
    QMessageBox::warning(this,tr("error"), err,QMessageBox::Yes);
}

/*yuv格式转换为rgb格式*/
int ProcessImage::convert_yuv_to_rgb_buffer(unsigned char *yuv, unsigned char *rgb, unsigned int width, unsigned int height)
{
    unsigned int in, out = 0;
    unsigned int pixel_16;
    unsigned char pixel_24[3];
    unsigned int pixel32;
    int y0, u, y1, v;
    for(in = 0; in < width * height * 2; in += 4) {
        pixel_16 =
                yuv[in + 3] << 24 |
                               yuv[in + 2] << 16 |
                                              yuv[in + 1] <<  8 |
                                                              yuv[in + 0];
        y0 = (pixel_16 & 0x000000ff);
        u  = (pixel_16 & 0x0000ff00) >>  8;
        y1 = (pixel_16 & 0x00ff0000) >> 16;
        v  = (pixel_16 & 0xff000000) >> 24;
        pixel32 = convert_yuv_to_rgb_pixel(y0, u, v);
        pixel_24[0] = (pixel32 & 0x000000ff);
        pixel_24[1] = (pixel32 & 0x0000ff00) >> 8;
        pixel_24[2] = (pixel32 & 0x00ff0000) >> 16;
        rgb[out++] = pixel_24[0];
        rgb[out++] = pixel_24[1];
        rgb[out++] = pixel_24[2];
        pixel32 = convert_yuv_to_rgb_pixel(y1, u, v);
        pixel_24[0] = (pixel32 & 0x000000ff);
        pixel_24[1] = (pixel32 & 0x0000ff00) >> 8;
        pixel_24[2] = (pixel32 & 0x00ff0000) >> 16;
        rgb[out++] = pixel_24[0];
        rgb[out++] = pixel_24[1];
        rgb[out++] = pixel_24[2];
    }
    return 0;
}
/*yuv格式转换为rgb格式*/
int ProcessImage::convert_yuv_to_rgb_pixel(int y, int u, int v)
{
    unsigned int pixel32 = 0;
    unsigned char *pixel = (unsigned char *)&pixel32;
    int r, g, b;
    r = y + (1.370705 * (v-128));
    g = y - (0.698001 * (v-128)) - (0.337633 * (u-128));
    b = y + (1.732446 * (u-128));
    if(r > 255) r = 255;
    if(g > 255) g = 255;
    if(b > 255) b = 255;
    if(r < 0) r = 0;
    if(g < 0) g = 0;
    if(b < 0) b = 0;
    pixel[0] = r * 220 / 256;
    pixel[1] = g * 220 / 256;
    pixel[2] = b * 220 / 256;
    return pixel32;
}

bool ProcessImage::eventFilter(QObject *watched, QEvent *event)
{
    static QPoint point;
    /*定义事件过滤器，指定在主窗口上检测*/
    if (watched == this && event->type() == QEvent::MouseButtonPress) {
        point.setY(cursor().pos().y());     // 记录按下点的y坐标
    }else if(watched == this && event->type() == QEvent::MouseButtonRelease){
        {
			if(cursor().pos().y() > (float)150/480*this->geometry().height())//防止用户滑动大于150以上的区域而触发关闭按出现
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
                QTimer::singleShot(3000, &eventloop, SLOT(quit())); // 延时3s后关闭
                eventloop.exec();
                widget->hide();
            }
        }
    }
    return QWidget::eventFilter(watched,event);//将事件传递给父类
}


