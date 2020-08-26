#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QKeyEvent>
#include <QDesktopWidget>
#include <QPushButton>
#include <QTimer>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setGeometry(0,0,QApplication::desktop()->screenGeometry().width(),
                      QApplication::desktop()->screenGeometry().height());

    this->setStyleSheet("QMainWindow{background-color:rgba(220,220,220,100%);}");
    this->setWindowFlags(Qt::FramelessWindowHint);
    lineEdit = new QLineEdit(this);
    lineEdit->setStyleSheet("QLineEdit{border-radius:3px}");//设置文本输入框的样式表
    lineEdit->setText("http://www.baidu.com");//默认网址为百度
    webView = new QWebView(this);
    page = new webPage(this);
    this->setCentralWidget(webView);
    webView->setPage(page);
    webView->installEventFilter(this);

    /* 添加动作图标与提示 */
    actionBack = new QAction(QIcon(":/icon/icon/back.png"),"后退到上一个网页ctrl+←",this);
    actionForward = new QAction(QIcon(":/icon/icon/forward.png"),"前进ctrl+→",this);
    actionLoadUrl = new QAction(QIcon(":/icon/icon/goto.png"),"转到Enter",this);
    actionClose = new QAction(QIcon(":/icon/icon/close.png"),"关闭ctrl+Q",this);
    actionZoomIn = new QAction(QIcon(":/icon/icon/zoom_in.png"),"放大ctrl+↑",this);
    actionZoomOut = new QAction(QIcon(":/icon/icon/zoom_out.png"),"缩小ctrl+↓",this);
    actionRefresh = new QAction(QIcon(":/icon/icon/refresh.png"),"刷新ctrl+R",this);
    actionHome = new QAction(QIcon(":/icon/icon/home.png"),"主页ctrl+H",this);

    actionZoomOut->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Down));
    actionZoomIn->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Up));
    actionClose->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Q));
    actionBack->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Left));
    actionForward->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Right));
    actionRefresh->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_R));
    actionHome->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_H));
    actionLoadUrl->setShortcut(QKeySequence(Qt::Key_Return));

    /* 将lineEdit与actionLoadUrl添加到mainToolBar */
    ui->mainToolBar->addAction(actionBack);
    ui->mainToolBar->addAction(actionForward);
    ui->mainToolBar->addAction(actionRefresh);
    ui->mainToolBar->addAction(actionHome);
    ui->mainToolBar->addWidget(lineEdit);
    ui->mainToolBar->addAction(actionLoadUrl);
    ui->mainToolBar->addAction(actionClose);
    ui->mainToolBar->addAction(actionZoomIn);
    ui->mainToolBar->addAction(actionZoomOut);


    connect(actionLoadUrl,SIGNAL(triggered()),this,SLOT(slotActionLoadUrlFunc()));
    connect(lineEdit,SIGNAL(returnPressed()),this,SLOT(slotActionLoadUrlFunc()));
    connect(page,SIGNAL(loadLink(QUrl)),this,SLOT(linkUrl(QUrl)));
    connect(page,SIGNAL(openLink(QUrl)),this,SLOT(openUrl(QUrl)));
    connect(actionBack,SIGNAL(triggered()),this,SLOT(slotUrlBack()));
    connect(actionForward,SIGNAL(triggered()),this,SLOT(slotUrlForward()));
    connect(actionClose,SIGNAL(triggered()),this,SLOT(slotClose()));
    connect(actionZoomIn,SIGNAL(triggered()),this,SLOT(slotZoomIn()));
    connect(actionZoomOut,SIGNAL(triggered()),this,SLOT(slotZoomOut()));
    connect(webView,SIGNAL(urlChanged(QUrl)),this,SLOT(slotUrlchanged()));
    connect(actionRefresh,SIGNAL(triggered()),this,SLOT(slotRefresh()));
    connect(actionHome,SIGNAL(triggered()),this,SLOT(slotHome()));
    /* 开机先执行一次显示网页 */
    slotActionLoadUrlFunc();

    ui->mainToolBar->installEventFilter(this);
}

void MainWindow::slotActionLoadUrlFunc()
{
    /* 使用setUrl函数加载网址，单行输入框的内容直接输入 */
    webView->setUrl(QUrl(lineEdit->text()));
}

void MainWindow::linkUrl(QUrl)
{
    /* 处理不打开新窗口的连接 */
}
void MainWindow::openUrl(QUrl url)
{
    /* 处理新窗口的连接 */
    webView->setUrl(url);
}

void MainWindow::slotUrlBack()
{
    webView->back();
}

void MainWindow::slotUrlForward()
{
    webView->forward();
}

void MainWindow::slotClose()
{
    this->close();
}

void MainWindow::slotZoomIn()
{
    qreal factor = webView->zoomFactor();
    webView->setZoomFactor(factor+0.1);
}

void MainWindow::slotZoomOut()
{
    qreal factor = webView->zoomFactor();
    webView->setZoomFactor(factor-0.1);
}

void MainWindow::slotUrlchanged()
{
    lineEdit->setText(webView->url().toString());
}

void MainWindow::slotRefresh()
{
    webView->reload();
}

void MainWindow::slotHome()
{
    webView->setUrl(QUrl("http://www.baidu.com"));
}

bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{
    /* 定义事件过滤器，指定在webView上检测 */
    if (watched == webView && event->type() == QEvent::MouseButtonDblClick) {
        static bool mainToolHideFlag = true;
        if(mainToolHideFlag){
            ui->mainToolBar->hide();
        }else{
            ui->mainToolBar->show();
        }
        mainToolHideFlag = !mainToolHideFlag;
    }

    if(watched == ui->mainToolBar){
        static QPoint point;
        switch (event->type()) {
        case QEvent::MouseButtonPress:
            point.setY(cursor().pos().y());     // 记录按下点的y坐标
            break;
        case QEvent::MouseButtonRelease:
        {
            int offsety = cursor().pos().y() - point.y();
            if(offsety > 2){ // 判断如果这个值大于2，就出现关闭按钮
                static QWidget *widget = new QWidget(this);
                static QPushButton *pushButton = new QPushButton(widget);
                pushButton->setFocusPolicy(Qt::NoFocus);
                widget->setStyleSheet("QWidget{border-radius:6px;background-color:rgba(100,100,100,80%);border-image:transparent}");
                widget->setGeometry(0,0,this->geometry().width(),40);
                pushButton->setGeometry(this->geometry().width()-40,0,40,40);
                pushButton->setStyleSheet("QPushButton{border-image:url(:/icon/icon/close2.png);"
                                          "background-color:transparent}"
                                          "QPushButton:hover{border-image:url(:/icon/icon/close2_hover.png);}"
                                          );
                connect(pushButton,SIGNAL(clicked()),this,SLOT(close()));
                widget->show();
                QEventLoop eventloop;
                QTimer::singleShot(3000, &eventloop, SLOT(quit())); // 延时3s
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

MainWindow::~MainWindow()
{
    delete ui;
    delete lineEdit;
    delete actionForward;
    delete actionBack;
    delete actionLoadUrl;
    delete actionClose;
    delete actionZoomOut;
    delete actionZoomIn;
    delete actionRefresh;
    delete actionHome;
    delete webView;
    delete page;
}
