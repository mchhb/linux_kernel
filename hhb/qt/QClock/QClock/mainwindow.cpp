#include "mainwindow.h"
#include <QtMath>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    /* 获取屏幕分辨率信息，设置主窗口的大小 */
    this->setGeometry(0,0,QApplication::desktop()->width(),QApplication::desktop()->height());
    this->setWindowFlags(Qt::FramelessWindowHint);          // 主窗口无边框
    this->setStyleSheet("QWidget{ border-image: url(Resource/image/qclockbg.png);}");

    /* Label初始化 */
    timeDisplay = new QLabel(this);
    dateDisplay = new QLabel(this);

    timeDisplay->setStyleSheet("color: white;border-image:transparent");
    dateDisplay->setStyleSheet("color: white;border-image:transparent");

    QFont font;
    font.setPixelSize((float)20/800*this->geometry().width());
    dateDisplay->setFont(font);
    font.setPixelSize((float)70/800*this->geometry().width());
    timeDisplay->setFont(font);

    /* 设置控件的位置 */
    timeDisplay->setGeometry((float)400/800*this->geometry().width(), (float)240/480*this->geometry().height(),
                             (float)385/800*this->geometry().width(), (float)120/480*this->geometry().height());
    dateDisplay->setGeometry((float)400/800*this->geometry().width(), (float)360/480*this->geometry().height(),
                             (float)380/800*this->geometry().width(), (float)120/480*this->geometry().height());
    timeDisplay->setAlignment(Qt::AlignRight | Qt::AlignBottom);  // 设置字体对其方式
    dateDisplay->setAlignment(Qt::AlignRight | Qt::AlignTop);
    timeDisplay->setText(QTime::currentTime().toString());        // 初始化Label显示
    dateDisplay->setText(QDate::currentDate().toString());

    /* 画笔参数初始化 */
    painPara.painterColor = QColor("#f96b22");
    painPara.translateX   = (float)200/800*this->geometry().width();
    painPara.translateY   = (float)240/480*this->geometry().height();
    painPara.scale        = (qMin((float)200/800*this->geometry().width(),
                                  (float)240/480*this->geometry().height())) / 100;
    painPara.coordinateHourHand[0] = QPoint(7,8);
    painPara.coordinateHourHand[1] = QPoint(-7,8);
    painPara.coordinateHourHand[2] = QPoint(0,-40);

    painPara.coordinateMinuteHand[0] = QPoint(7,8);
    painPara.coordinateMinuteHand[1] = QPoint(-7,8);
    painPara.coordinateMinuteHand[2] = QPoint(0,-70);

    painPara.coordinateSecondHand[0] = QPoint(7,8);
    painPara.coordinateSecondHand[1] = QPoint(-7,8);
    painPara.coordinateSecondHand[2] = QPoint(0,-80);

    /* 定时器初始化 */
    timer      = new QTimer(this);
    timerClose = new QTimer(this);
    timer->start(1000);           // 开始计时，1秒一次

    /* 信号、槽连接 */
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    connect(timerClose, SIGNAL(timeout()), this, SLOT(timerCloseTimeOut()));
}

MainWindow::~MainWindow()
{
    timer->stop();
    timerClose->stop();
    delete timer;
    delete timerClose;
    delete timeDisplay;
    delete dateDisplay;
}

void MainWindow::mousePressEvent(QMouseEvent *event) {
    Q_UNUSED(event);
    mousePoint.setY(cursor().pos().y());// 记录按下点y坐标值
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event) {
    Q_UNUSED(event);
    int previousPoint = mousePoint.y();     // 获取按下点y坐标值
    mousePoint.setY(cursor().pos().y());    // 获取释放点的y坐标值
	if(cursor().pos().y() > (float)150/480*this->geometry().height())//防止用户滑动大于150以上的区域而触发关闭按出现
		return;
    int offset =   mousePoint.y() - previousPoint;
    if(offset > 20)
    {
        static QWidget *widget = new QWidget(this);
        static QPushButton *pushButton = new QPushButton(widget);
        pushButton->setFocusPolicy(Qt::NoFocus);
        widget->setStyleSheet("QWidget{border-radius:6px;background-color:rgba(100,100,100,80%);border-image:transparent}");
        widget->setGeometry(0,0,this->geometry().width(),40);
        pushButton->setGeometry(this->geometry().width()-40,0,40,40);
        pushButton->setStyleSheet("QPushButton{border-image:url(:/icon/Resource/icon/close.png);"
                                  "background-color:transparent}"
                                  "QPushButton:hover{border-image:url(:/icon/Resource/icon/close_hover.png);}"
                                  );
        connect(pushButton,SIGNAL(clicked()),this,SLOT(close()));
        widget->show();
        QEventLoop eventloop;
        QTimer::singleShot(3000, &eventloop, SLOT(quit())); // 延时3s
        eventloop.exec();
        widget->hide();
    }
}

void MainWindow::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);

    QPainter painter(this);
    painter.translate(painPara.translateX, painPara.translateY);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.scale(painPara.scale, painPara.scale);
    painter.setPen(painPara.painterColor);
    painter.setBrush(painPara.painterColor);

    /* 获取系统时间 */
    QTime time = QTime::currentTime();

    /* 时针 */
    painter.save();
    painter.rotate(30.0 * (time.hour() + time.minute() / 60.0)); //角度转换30度
    painter.drawConvexPolygon(painPara.coordinateHourHand, 3);   //描画的形状
    painter.restore();

    /* 描画时针刻度线 */
    painter.save();

    /* 描画时钟数字文本 */
    int nHour = 0;
    for (int i = 0; i < 12; ++i) {
        nHour = i + 3;
        if (nHour > 12)
            nHour -= 12;
        painter.drawText(textRectF(75, 14, i * 30), Qt::AlignCenter, QString::number(nHour));
    }

    for(int i = 0; i < 12; i++)
    {
        painter.rotate(30.0);
        painter.drawLine(80,0,90,0);
    }
    painter.restore();

    /* 分针 */
    painter.save();
    painter.rotate(6.0 * (time.minute() + time.second() / 60.0));
    painter.drawConvexPolygon(painPara.coordinateMinuteHand, 3);
    painter.restore();

    /* 描画分针刻度线 */
    painter.save();
    for(int i = 0; i<60;i++) {
        if(i % 5)
            painter.drawLine(82,0,87,0);
        painter.rotate(6.0);
    }
    painter.restore();

    /* 秒针 */
    painter.save();
    painter.rotate(6.0 * time.second());
    painter.drawConvexPolygon(painPara.coordinateSecondHand, 3);
    painter.restore();

    /* 更新时间、日期显示 */
    timeDisplay->setText(time.toString());
    dateDisplay->setText(QDate::currentDate().toString());
}

/* 参考一去二三里的方法 */
QRectF MainWindow::textRectF(double radius, int pointSize, double angle)
{
    QRectF rectF;
    rectF.setX(radius*qCos(angle*M_PI/180.0) - pointSize*2);
    rectF.setY(radius*qSin(angle*M_PI/180.0) - pointSize/2.0);
    rectF.setWidth(pointSize*4);
    rectF.setHeight(pointSize);
    return rectF;
}
