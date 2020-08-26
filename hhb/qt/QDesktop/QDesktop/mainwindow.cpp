#include "mainwindow.h"
#include "weather.h"
#include <QDesktopWidget>
#include <QApplication>
#define   DEFAULT_IMAGE_FILEPATH   "./Resource/images"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    /* 主窗口初始化 */
    mainWindowInit();

    /* 普通变量初始化 */
    normalVarInit();

    /* 顶部图标初始化 */
    topIconInit();

    /* 时间、日期显示初始化 */
    datetimeDisInit();

    /* 天气初始化 */
    weatherDisInit();

    /* image */
    imageDisInit();

    /* 网络访问请求初始化 */
    networkAccessManagerInit();

    /* 定时器初始化 */
    timerInit();

    /* app初始化 */
    appIconInit();

    /* 信号、槽函数连接 */
    connect(this, SIGNAL(signalUpdateWeather()), this, SLOT(slotUpdateWeather()));
}

MainWindow::~MainWindow()
{
    /* 定时器 */
    delete mainTimer;
    delete autoUpdateWeatherTimer;

    /* app */
    appArr.clear();

    delete appBG;

    /* 网络 */
    delete NAManager;

    /* 顶部 */
    qDeleteAll(topIcon);
    topIcon.clear();
    delete topTimeDis;

    /* 左侧 */
    delete displayAMorPM;
    delete displayDate;
    delete displayTime;
    delete dateTimeDisAreaBG;

    delete weatherIcon;
    delete displayWeather;
    delete displayCity;
    delete displayTemp;
    delete displayMinMaxTemp;
    delete weatherDisAreaBG;

    delete displayImage;
    imageArr.clear();
}


/*
 * 私有方法
 */
void MainWindow::mainWindowInit(void) {
    //this->setGeometry(0,0, 800, 480);
    this->setGeometry(0,0,QApplication::desktop()->width(),QApplication::desktop()->height());
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setStyleSheet("QMainWindow {border-image: url(Resource/background/bgDesktop_3.png);}");
    this->installEventFilter(this); // 安装事件过滤器

    userLabel = new QLabel(this);
    userLabel->setGeometry((float)10/800*this->geometry().width(),(float)5/480*this->geometry().height(),
                           (float)25/800*this->geometry().width(),(float)25/800*this->geometry().width());
    userLabel->setStyleSheet("QLabel {border-image: url(:/icon/Resource/icon/user.png);}");
}

void MainWindow::normalVarInit(void) {
    /* 标志位 */
    flagAppExec             = true;
    flagUpdateWeather       = false;
}

void MainWindow::topIconInit(void) {
    topIcon.clear();
    QFont font;

    /* 实例化 */
    for (int i = 0; i < 1; i++) {
        QWidget *icon = new QWidget(this);
        topIcon.append(icon);
    }

    /* 坐标 */
    topIcon[0]->setGeometry((float)763/800*this->geometry().width(), (float)2/480*this->geometry().height(),
                            (float)27/800*this->geometry().width(), (float)20/480*this->geometry().height());

    /* 样式表设置 */
    topIcon[0]->setStyleSheet("QWidget {border-image: url(:/icon/Resource/icon/iconWifi); "
                              "background-color: rgba(0, 0, 0, 0%)}");

    /* 时间显示初始化 */
    topTimeDis = new QLabel(this);
    topTimeDis->setGeometry((float)360/800*this->geometry().width(), (float)2/480*this->geometry().height(),
                            (float)80/800*this->geometry().width(), (float)20/480*this->geometry().height());
    topTimeDis->setStyleSheet("QLabel {color: white;background: transparent}");
    topTimeDis->setAlignment(Qt::AlignCenter);
    topTimeDis->setText(QTime::currentTime().toString().mid(0, 8));
    font.setPointSize((float)14/800*this->geometry().width());
    topTimeDis->setFont(font);
}

void MainWindow::datetimeDisInit(void) {
    QFont font;
    /* 区域背景显示 */
    dateTimeDisAreaBG = new QWidget(this);
    dateTimeDisAreaBG->setGeometry((float)20/800*this->geometry().width(), (float)60/480*this->geometry().height(),
                                   (float)220/800*this->geometry().width(), (float)120/480*this->geometry().height());
    dateTimeDisAreaBG->setStyleSheet("QWidget {border-image: url(Resource/background/bgTime);"
                                     "background-color: rgba(0, 0, 0, 0%);}");

    /* 时间 */
    QTime currentTime = QTime::currentTime();
    displayTime = new QLabel(this);
    displayTime->setGeometry((float)20/800*this->geometry().width(), (float)60/480*this->geometry().height(),
                             (float)180/800*this->geometry().width(), (float)80/480*this->geometry().height());
    displayTime->setStyleSheet("QLabel { color: white;background: transparent}");
    displayTime->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
    displayTime->setText(currentTime.toString().mid(0, 5));
    font.setPointSize((float)48/800*this->geometry().width());
    displayTime->setFont(font);

    /* AM?  PM? */
    displayAMorPM = new QLabel(this);
    displayAMorPM->setGeometry((float)200/800*this->geometry().width(), (float)100/480*this->geometry().height(),
                               (float)40/800*this->geometry().width(), (float)40/480*this->geometry().height());
    displayAMorPM->setStyleSheet("QLabel {color: white;background: transparent}");
    displayAMorPM->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    font.setPointSize((float)15/800*this->geometry().width());
    displayAMorPM->setFont(font);

    if (12 <= currentTime.hour())
        displayAMorPM->setText("PM");
    else
        displayAMorPM->setText("AM");

    /* 日期 */
    displayDate = new QLabel(this);
    displayDate->setGeometry((float)30/800*this->geometry().width(), (float)140/480*this->geometry().height(),
                             (float)200/800*this->geometry().width(), (float)40/480*this->geometry().height());
    displayDate->setStyleSheet("QLabel { color: white;background: transparent}");
    displayDate->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    displayDate->setText(QDate::currentDate().toString("MMMM dd dddd"));
    font.setPointSize((float)12/800*this->geometry().width());
    displayDate->setFont(font);
}

void MainWindow::imageDisInit(void) {
    imageArr.clear();

    /* 扫描目录添加文件 */
    QDir dir(DEFAULT_IMAGE_FILEPATH);
    if (dir.exists()) {

        QStringList filter;             // 定义一个过滤器
        filter << "*.png";              // 添加需要扫描的文件类型
        QFileInfoList files = dir.entryInfoList(filter, QDir::Files);

        for (int i = 0; i < files.count(); i++) {
            QString path = files.at(i).filePath();
            imageArr.append(path);      //  将image路径添加到数组
        }
    }

    photoAlbum = new QWidget(this);
    photoAlbum->setGeometry((float)20/800*this->geometry().width(), (float)315/480*this->geometry().height(),
                            (float)220/800*this->geometry().width(), (float)150/480*this->geometry().height());

    /* 判断是否为空 */
    if (!imageArr.isEmpty())
        photoAlbum->setStyleSheet("QWidget{border-image:url("+ imageArr[0] +")}");


    /* 对象初始化，相册外框 */
    displayImage = new QWidget(this);
    displayImage->setGeometry((float)20/800*this->geometry().width(), (float)315/480*this->geometry().height(),
                              (float)220/800*this->geometry().width(), (float)150/480*this->geometry().height());
    displayImage->setStyleSheet("QWidget {border:10px; border-style:outset; padding:0px; margin:0px;"
                                "border-color: rgba(255, 255, 255, 50%);}");
    displayImage->installEventFilter(this); // 安装事件过滤器，当点击相册时，打开相册程序
}

void MainWindow::timerInit(void) {
    /* 实例化 */
    mainTimer               = new QTimer(this);
    autoUpdateWeatherTimer  = new QTimer(this);
    appExecTimer            = new QTimer(this);
    autoUpdateImageTimer    = new QTimer(this);
    pageTimer               = new QTimer(this);

    /* 信号与槽函数连接 */
    connect(mainTimer, SIGNAL(timeout()), this, SLOT(slotTimeoutMainTimer()));
    connect(autoUpdateWeatherTimer, SIGNAL(timeout()), this, SLOT(slotTimeoutAutoUpdateWeather()));
    connect(appExecTimer, SIGNAL(timeout()), this, SLOT(slotTimeroutExecTimer()));
    connect(autoUpdateImageTimer, SIGNAL(timeout()), this, SLOT(slotTimerouUpdateImageTimer()));
    connect(pageTimer, SIGNAL(timeout()), this, SLOT(slotTimeoutPageTimer()));

    /* 开启定时器 */
    mainTimer->start(1000);   // 1ms秒更新一次
    autoUpdateWeatherTimer->start(1000 * 60 * 20);  // 20分钟更新一次天气显示 这个接口一天的请求次数有限制
    if (2 <= imageArr.count()) // 判读当前是否扫描到了多张image 如果小于2则不用开启
        autoUpdateImageTimer->start(100);
}
/* 更新时间和日期 */
void MainWindow::slotTimeoutMainTimer(void) {
    QTime currentTime = QTime::currentTime();
    QString time = currentTime.toString().mid(0, 5);

    /* 设置时间 */
    displayTime->setText(time);
    time = currentTime.toString().mid(0, 8);
    topTimeDis->setText(time);

    /* AM? PM? */
    if (12 <= currentTime.hour())
        displayAMorPM->setText("PM");
    else
        displayAMorPM->setText("AM");

    /* 更新日期 */
    displayDate->setText(QDate::currentDate().toString("MMMM dd dddd"));

}

void MainWindow::slotTimerouUpdateImageTimer()
{
    static int i   = 0;
    static qreal j = 0.4;
    static int k   = 0;
    static QGraphicsOpacityEffect  *opacityEffect = new QGraphicsOpacityEffect(); // 设置图标的不透明度
    i++;
    if(i == 30){ // 7s
        k++;
        if(k == imageArr.count()){
            k = 0;
        }
        j = 0.2;
        photoAlbum->setGraphicsEffect(opacityEffect);
        photoAlbum->setStyleSheet("QWidget{border-image:url("+ imageArr[k] +")}");
    }
    if(i == 50) // 10s
        i = 0;

    if (j >= 0.2)
        j = j + 0.1;
    opacityEffect->setOpacity(j);
}
/* 自动更新天气 */
void MainWindow::slotTimeoutAutoUpdateWeather(void) {
    /* 如果当前正处于更新状态 则退出 因为手动更新可能会与自动更新时间上存在冲突 */
    if (!flagUpdateWeather)
        return;

    flagUpdateWeather = false;
    QNetworkRequest  ipGetNRequest;
    ipGetNRequest.setUrl(QUrl(IP_GET_API_URL));

    /* get */
    QNetworkReply *reply = NAManager->get(ipGetNRequest);

    /* 连接信号、槽函数 */
    connect(reply, SIGNAL(finished()), this, SLOT(slotFinishedIpGetReply()));
}
/* 防止阻塞后误操作 */
void MainWindow::slotTimeroutExecTimer()
{
    appExecTimer->stop();
    flagAppExec = true;
}

/* 更新天气 */
void MainWindow::slotUpdateWeather(void) {
    displayCity->setText("正在定位...");

    QNetworkRequest  ipGetNRequest;
    ipGetNRequest.setUrl(QUrl(IP_GET_API_URL));

    /* get */
    QNetworkReply *reply = NAManager->get(ipGetNRequest);

    /* 连接信号、槽函数 */
    connect(reply, SIGNAL(finished()), this, SLOT(slotFinishedIpGetReply()));
}
/* 页面滑动计算 */
void MainWindow::slotTimeoutPageTimer()
{
    static int distanceStep = (float)80/800*this->geometry().width();
    if(appBGScrollArea->horizontalScrollBar()->value()<appBGScrollArea->horizontalScrollBar()->maximum()/2){
        distance = distance - distanceStep;
        if(distance <= 0)
        {
            pageTimer->stop();
            distance = 0;
        }
        appBGScrollArea->horizontalScrollBar()->setValue(distance);
    }else{

        if(distance >= appBGScrollArea->horizontalScrollBar()->maximum())
        {
            pageTimer->stop();
            distance = appBGScrollArea->horizontalScrollBar()->maximum();
        }
        distance = distance + distanceStep;
        appBGScrollArea->horizontalScrollBar()->setValue(distance);
    }

}

