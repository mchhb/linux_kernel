#ifndef CAMERA_H
#define CAMERA_H
#include <QDialog>
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPropertyAnimation>
#include <QFrame>
#include <QTimer>
#include <QComboBox>
#include "v4l2camera.h"

class camera : public QDialog
{
   Q_OBJECT

public:
    camera(QWidget *parent=0);
    ~camera();

private slots:
    void closeApp();    //真正关闭窗口的函数

protected slots:
    int slotBeginCapture();    //开始捕获槽函数
    int slotSavePicture();
    void slotExitCamera();
    int slotFlushBuff();
    void slotFrameRateTimer();
    void slotCameraSelect(const QString &);
    void slotResolutionSelect(const QString &);

private:
    int imagewidth;  //宽度
    int imageheight; //高度
    int frameRate;  //帧率
    QString devname;
    V4l2Camera *v4l2camera;
    pass_data pd;                   //摄像头相关结构体
    unsigned char *bufrgb;          //接收到的摄像头数据缓冲区
    QLabel *cameraSelectLabel;
    QLabel *resolutionLabel;        //分辨率标签
    QLabel *displayLabel;           //图像显示label
    QLabel *frameNameLabel;         //帧率显示label
    QLabel *frameRateLabel;         //帧率显示label
    QComboBox *cameraSelect;        //摄像头选择下拉列表
    QComboBox *resolutionSelect;    //分辨率选择下拉列表
    QHBoxLayout *controlLayout;     //控制按钮布局
    QVBoxLayout *mainLayout;

    //QFrame
    QFrame *topFrame;
    QFrame *midFrame;
    QFrame *bottomFrame;

    //Button
    QPushButton *beginBtn;  //开始
    QPushButton *saveBtn;   //拍照
    QPushButton *exitBtn;   //退出
    QTimer *timer;
    QTimer *frameTimer;     //帧率统计时钟

    bool eventFilter(QObject *watched, QEvent *event);



};

#endif // CAMERA_H
