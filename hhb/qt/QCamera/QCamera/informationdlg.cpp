#include "informationdlg.h"

InformationDlg::InformationDlg(QWidget *parent, const QString &title,const QString& text):QDialog(parent),
    QuesstionDlgtitle(title),QuesstionDlgText(text)
{
    QFont font("STKaiti",16);
    //setAttribute(Qt::WA_DeleteOnClose,true); //关闭窗口释放内存

    this->setModal(true);//设置为模态对话框
    setFixedSize(300,200);
    setWindowFlags(Qt::FramelessWindowHint | Qt::Tool);
    //setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground); //设置为透明颜色

    frame = new QFrame(this);
    frame->setGeometry(0,0,300,200);
    frame->setStyleSheet("QFrame{background-color:#6495ED;"
                        "border:none;"
                        "border-radius:10px}");

    textEdit = new QTextEdit();
    textEdit->resize(300,200);
    textEdit->setReadOnly(true);
    textEdit->setStyleSheet("QTextEdit{border:2px;}");
    textEdit->setText(QuesstionDlgText);
    textEdit->setFont(font);
    textEdit->setAlignment(Qt::AlignVCenter|Qt::AlignHCenter);

    okBtn = new QPushButton(tr("确定"));
    okBtn->setFont(font);
    okBtn->setStyleSheet("QPushButton{border:1px solid #8f8f91;"
                       "border-radius:5px;"
                       "background-color:#BFBFBF;"
                       "min-height:30px;"
                       "outline:none;}"
                       "QPushButton:pressed{background-color:#8E8E8E;"
                       "border:1px solid #8f8f91;"
                       "color:white;}"
                       "QPushButton:hover{background-color:#8E8E8E;"
                       "color:white;}");

     mainLayout = new QVBoxLayout(frame);
     mainLayout->addWidget(textEdit);
     mainLayout->addWidget(okBtn);

    connect(okBtn, SIGNAL(clicked()),this, SLOT(slotOkBtn()));
}

int InformationDlg::slotOkBtn()
{
    close();
}
