#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "config.h"
#include <QMenu>
#include <QMenuBar>
#include <QMainWindow>
#include <QTextEdit>
#include <QLabel>
#include <QMessageBox>
#include <QFileDialog>
#include <QTextStream>
#include <QCloseEvent>
#include <QScrollBar>
#include <QDesktopWidget>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    void mainWindowInit();

    QMenuBar *menu;

    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *helpMenu;

    QAction *newAct;    //新建
    QAction *openAct;   //打开
    QAction *saveAct;   //保存
    QAction *saveAsAct; //另存为
    QAction *quitAct;   //退出

    QAction *undoAct;   //撤销
    QAction *redoAct;   //重做
    QAction *cutAct;    //剪切
    QAction *copyAct;   //复制
    QAction *pasteAct;  //粘贴

    QAction *aboutAct;
    /* 文本编辑区 */
    QTextEdit *textEdit;
    QLabel *textInfo;

    QString currentFile;

    double heightFactor;
    double widthFactor;


    bool maybeSave();
    bool saveFile(QString fileName);
    void setFileName(QString fileName);
    void loadFile(QString fileName);

    void closeEvent(QCloseEvent *e);
    bool eventFilter(QObject *watched, QEvent *event);
private slots:
    void slotshowTextInfo();

    void slotCut();
    void slotCopy();
    void slotPaste();
    void slotRedo();
    void slotUndo();
    void slotactAbout();

    void slotNew();
    bool slotSave();
    bool slotSaveAs();
    void slotOpen();
    void slotquitApp();
};

#endif // MAINWINDOW_H
