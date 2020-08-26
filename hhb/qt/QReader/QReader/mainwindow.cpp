#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QPushButton>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    mainWindowInit();
}

void MainWindow::mainWindowInit()
{
    this->setWindowTitle("小说图书/阅读器");
    this->setWindowFlags(Qt::FramelessWindowHint);
    menu = new QMenuBar(this);
    menu->installEventFilter(this);

    fileMenu = new QMenu(this);
    helpMenu = new QMenu(this);
    readingMenu = new QMenu(this);

    fileMenu = menu->addMenu("文件");
    helpMenu = menu->addMenu("帮助");
    readingMenu = menu->addMenu("阅读模式");
    openAct = new QAction(QIcon(""),"打开",this);
    openAct->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_O));
    openAct->setStatusTip("打开");
    quitAct = new QAction(QIcon(""),"退出",this);
    quitAct->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Q));
    quitAct->setStatusTip("关闭");

    fileMenu->addAction(openAct);
    fileMenu->addAction(quitAct);

    aboutAct = new QAction(QIcon(""),"关于",this);
    aboutAct->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_H));
    aboutAct->setStatusTip("关于");

    helpMenu->addAction(aboutAct);

    normalModelAct = new QAction(QIcon(""),"经典模式",this);
    normalModelAct->setStatusTip("经典模式");
    greenEyeModelAct = new QAction(QIcon(""),"绿色护眼",this);
    greenEyeModelAct->setStatusTip("绿色护眼");
    nightModelAct = new QAction(QIcon(""),"夜间模式",this);
    nightModelAct->setStatusTip("夜间模式");
    pergamynModelAct = new QAction(QIcon(""),"羊皮纸",this);
    pergamynModelAct->setStatusTip("羊皮纸");

    readingMenu->addAction(normalModelAct);
    readingMenu->addAction(greenEyeModelAct);
    readingMenu->addAction(nightModelAct);
    readingMenu->addAction(pergamynModelAct);

    /* 工具栏 */
    toolBar = new QToolBar(this);

    colorAct = new QAction(QIcon(":/icon/icon/A.png"),"字体颜色",this);
    fontZoomInAct = new QAction(QIcon(":/icon/icon/A+.png"),"字体增大",this);
    fontZoomOutAct = new QAction(QIcon(":/icon/icon/A-.png"),"字体减小",this);

    toolBar->addAction(colorAct);
    toolBar->addAction(fontZoomInAct);
    toolBar->addAction(fontZoomOutAct);
    this->toolBar->installEventFilter(this);//安装事件过滤器，用于监测事件，关闭整个窗口


    textBrowser = new QTextBrowser(this);
    textBrowser->verticalScrollBar()->setStyleSheet(TEXTBROWSER_STYLE);
    /* 行号与行字数lable */
    textInfo = new QLabel(this);

    textInfo->setText("Ready");//设置默认显示内容

    this->setGeometry(0,0,QApplication::desktop()->screenGeometry().width(),QApplication::desktop()->screenGeometry().height());

    menu->setGeometry(0,0,this->geometry().width(),30);
    toolBar->setGeometry(0,30,this->geometry().width(),30);
    textBrowser->setFont(QFont("宋体", 15));//设置默认字体：字体，字体大小
    textBrowser->setGeometry(0,60,this->geometry().width(),this->geometry().height()-80);//设置文本编辑区的大小与位置
    textInfo->setGeometry(this->geometry().width()-100,this->geometry().height()-20,100,20);
    textInfo->setAlignment(Qt::AlignCenter);

    connect(aboutAct,SIGNAL(triggered()),this,SLOT(slotactAbout()));
    connect(openAct,SIGNAL(triggered()),this,SLOT(slotOpen()));
    connect(quitAct,SIGNAL(triggered()),this,SLOT(slotQuit()));
    connect(normalModelAct,SIGNAL(triggered()),this,SLOT(slotactNormalReading()));
    connect(greenEyeModelAct,SIGNAL(triggered()),this,SLOT(slotactgreenEyeReading()));
    connect(nightModelAct,SIGNAL(triggered()),this,SLOT(slotactnightReading()));
    connect(pergamynModelAct,SIGNAL(triggered()),this,SLOT(slotactpergamynReading()));
    connect(textBrowser,SIGNAL(cursorPositionChanged()),this,SLOT(slotshowTextInfo()));
    connect(colorAct,SIGNAL(triggered()),this,SLOT(slotGetcolor()));
    connect(fontZoomInAct,SIGNAL(triggered()),this,SLOT(slotFontZoomIn()));
    connect(fontZoomOutAct,SIGNAL(triggered()),this,SLOT(slotFontZoomOut()));
}
void MainWindow::slotOpen()
{
    if(1){
        QString fileName = QFileDialog::getOpenFileName(this);
        if(!fileName.isEmpty()){
            loadFile(fileName);
        }
    }
}

void MainWindow::slotQuit()
{
    qApp->closeAllWindows();
}

void MainWindow::slotactAbout()
{
    QMessageBox box;
    box.about(this,"About","Author@Day");
    box.setFocusPolicy(Qt::NoFocus);
}

/* 加载文件的函数 */
void MainWindow::loadFile(QString fileName)
{
    QFile file(fileName);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        QMessageBox::critical(this,
                              "critical",
                              "cannot read file"
                              );
    }else{
        QTextStream in(&file);
        textBrowser->setText(in.readAll());
    }
}
/* 获取鼠标当前位置函数 */
void MainWindow::slotshowTextInfo()
{
    QTextCursor textCursor = textBrowser->textCursor();//获得鼠标光标
    int lineNum = textCursor.blockNumber();//行数
    int colNum = textCursor.columnNumber();//列数
    textInfo->setText(tr("行:%1,字:%2").arg(lineNum+1).arg(colNum));//显示在Label上，注意：行数是从0开始的
}

void MainWindow::slotGetcolor()
{
    QColor color = QColorDialog::getColor(Qt::red,this);
    if(color.isValid()){
        QTextCharFormat fmt;
        fmt.setForeground(color);
        QTextCursor textCursor = textBrowser->textCursor();//获取鼠标是否有选中文件
        if(textCursor.hasSelection()== false){
            textBrowser->selectAll();
        }
        textBrowser->mergeCurrentCharFormat(fmt);
        textCursor.clearSelection();
        textBrowser->setTextCursor(textCursor);
    }
}

void MainWindow::slotactNormalReading()
{
    textBrowser->setStyleSheet("background-color: rgba(255, 255, 255, 80%)");
}

void MainWindow::slotactgreenEyeReading()
{
    textBrowser->setStyleSheet("background-color: rgba(200, 250, 175, 80%)");
}

void MainWindow::slotactnightReading()
{
    textBrowser->setStyleSheet("background-color: rgba(200, 200, 200, 80%)");
}

void MainWindow::slotactpergamynReading()
{
    textBrowser->setStyleSheet("background-color: rgba(240, 240, 150, 80%)");
}

void MainWindow::slotFontZoomIn()
{
    textBrowser->zoomIn(1);
}

void MainWindow::slotFontZoomOut()
{
    textBrowser->zoomOut(1);
}

bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{
    if(watched == toolBar || watched == menu){
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
                pushButton->setStyleSheet("QPushButton{border-image:url(:/icon/icon/close.png);"
                                          "background-color:transparent}"
                                          "QPushButton:hover{border-image:url(:/icon/icon/close_hover.png);}"
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
    /*注意删除时的先后顺序，先删除子，再到父*/
    delete ui;
    delete openAct;
    delete aboutAct;
    delete normalModelAct;
    delete greenEyeModelAct;
    delete nightModelAct;
    delete pergamynModelAct;
    delete colorAct;
    delete fontZoomInAct;
    delete fontZoomOutAct;
    delete toolBar;
    delete textInfo;
    delete fileMenu;
    delete helpMenu;
    delete menu;
    delete textBrowser;
}


