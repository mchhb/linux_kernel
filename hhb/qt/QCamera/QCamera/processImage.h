#ifndef PROCESSIMAGE_H
#define PROCESSIMAGE_H
#include <QWidget>
#include <QtGui>
#include "videodevice.h"
#include <QLabel>
#include <QPushButton>
class ProcessImage : public QWidget
{
    Q_OBJECT
public:
    ProcessImage(QWidget *parent=0);
    ~ProcessImage();

private:
    QLabel *label;
    QImage *frame;
    int rs;
    uchar *pp;
    uchar * p;
    unsigned int len;
    int convert_yuv_to_rgb_pixel(int y, int u, int v);
    int convert_yuv_to_rgb_buffer(unsigned char *yuv, unsigned char *rgb, unsigned int width, unsigned int height);
    VideoDevice *vd;
    QTimer *timer;
    bool eventFilter(QObject *watched, QEvent *event);//事件过滤器处理函数
    bool flag;

private slots:
    void paintEvent(QPaintEvent *);
    void display_error(QString err);
};

#endif
