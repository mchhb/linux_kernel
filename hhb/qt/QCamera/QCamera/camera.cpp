#include "camera.h"
#include <QDebug>
#include <QFont>

camera::camera(QWidget *parent):QDialog(parent)
{
    setFixedSize(1024,600);//设置大小为1024x600,禁止改变窗口大小

    // 界面动画
    animation = new QPropertyAnimation(this,"windowOpacity");
    animation->setDuration(500);
    animation->setStartValue(0);
    animation->setEndValue(1);
    animation->start();

    //创建三个Frame
    topFrame = new QFrame(this);
    topFrame->setStyleSheet("border:2px ");
    topFrame->setGeometry(0,0,1024,48);

    midFrame = new QFrame(this);
    midFrame->setStyleSheet("QFrame{border:2px;}");
    midFrame->setGeometry(0,48,1024,504);

    bottomFrame = new QFrame(this);
    bottomFrame->setStyleSheet("border:2px ");
    bottomFrame->setGeometry(0,552,1024,48);

    //关闭返回按钮
    closeBtn = new QPushButton(topFrame);
    closeBtn->setGeometry(0,0,48,48);  //位置在(0,0)
    closeBtn->setIcon(QPixmap(":/images/images/toolicon/return.png"));
    closeBtn->setIconSize(QSize(48,48));
    closeBtn->setStyleSheet("QPushButton{border-style:none;}");
    connect(closeBtn,SIGNAL(clicked()),this,SLOT(closeCamera()));


    label1 = new QLabel(midFrame);
    label1->setText(tr("这是一个摄像头窗口"));
    QFont font("Microsoft Yahei",20);
    label1->setFont(font);
    label1->setAlignment(Qt::AlignCenter);  //水平居中对齐

    mainLayout = new QVBoxLayout(midFrame);
    mainLayout->addWidget(label1);
}

void camera::closeCamera()
{
    animation->setDuration(500);
    animation->setStartValue(1);
    animation->setEndValue(0);
    animation->start();
    connect(animation, SIGNAL(finished()), this, SLOT(closeApp()));
}

void camera::closeApp()
{
    close();    //关闭窗口
}
