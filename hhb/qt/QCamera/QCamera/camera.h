#ifndef CAMERA_H
#define CAMERA_H
#include <QDialog>
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QPropertyAnimation>
#include <QFrame>
#include  "camera_app.h"
class camera : public QDialog
{
   Q_OBJECT

public:
    camera(QWidget *parent=0);

private slots:
    void closeCamera(); //关闭窗口，用来执行界面淡出效果
    void closeApp();    //真正关闭窗口的函数

private:
    QLabel *label1;
    QPushButton *closeBtn;
    QVBoxLayout *mainLayout;

    //动画
    QPropertyAnimation *animation;

    //QFrame
    QFrame *topFrame;
    QFrame *midFrame;
    QFrame *bottomFrame;


};

#endif // CAMERA_H
