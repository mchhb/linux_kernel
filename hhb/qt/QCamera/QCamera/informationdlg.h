#ifndef INFORMATIONDLG_H
#define INFORMATIONDLG_H
#include <QDialog>
#include <QWidget>
#include <QString>
#include <QFrame>
#include <QLabel>
#include <QTextEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QGridLayout>

class InformationDlg : public QDialog
{
    Q_OBJECT

public:
   InformationDlg(QWidget *parent=nullptr, const QString &title=QString(),const QString& text=QString());
   inline static int information(QWidget *parent, const QString &title,const QString& text)
   {
       InformationDlg messageBox(parent,title,text);
       messageBox.exec();
   }

private slots:
   int slotOkBtn();

private:
   QString QuesstionDlgtitle;
   QString QuesstionDlgText;
   QFrame *frame;

   QPushButton *okBtn;
   QTextEdit *textEdit;
   QVBoxLayout *mainLayout;
};

#endif // INFORMATIONDLG_H
