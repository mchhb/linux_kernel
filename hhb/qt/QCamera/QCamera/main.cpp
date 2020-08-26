#include "camera_app.h"
#include <QApplication>
#include <QTextCodec>
#include <QDir>
#include <QSplashScreen>
#include <QPixmap>
#include <QDebug>
#include <QElapsedTimer>
#include <QStyleFactory>
#include <QDateTime>
#include <QMovie>
int main(int argc, char *argv[])
{
    /* 参考https://blog.csdn.net/chenlong12580/article/details/23713025 */
    QApplication app(argc, argv);
    /* 设置编码格式为utf-8 默认的情况下 无法识别中文路径 所以这里必须要设置为utf-8 */
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));

    /* 设置程序的工作目录为可执行文件所在目录 */
    QDir::setCurrent(QCoreApplication::applicationDirPath());

    QPixmap pixmap("../common/loadingbg.png");
    QSplashScreen *screen = new QSplashScreen(pixmap);
    QMovie *myMovie = new QMovie("../common/loading.gif");
    QLabel *movieLabel = new QLabel(screen);
    movieLabel->setMovie(myMovie);
    movieLabel->setGeometry(520,280,260,260);
    myMovie->start();
    movieLabel->show();

    screen->setCursor(Qt::BlankCursor);// 隐藏鼠标
    screen->show();
#if 0
    int delayTime = 5;
    QElapsedTimer timer;
    timer.start();
    while(timer.elapsed() < (delayTime * 1000))
    {
         app.processEvents();
    }
#endif

#if 1
    QDateTime n=QDateTime::currentDateTime();
    QDateTime now;
    do{
        now=QDateTime::currentDateTime();
        app.processEvents();
        screen->show();
    } while (n.secsTo(now)<=1);// 1为需要延时的秒数
#endif

    camera w;
    w.show();

    screen->finish(&w);
    /* 停止动画 */
    myMovie->stop();
    movieLabel->hide();
    return app.exec();
}

