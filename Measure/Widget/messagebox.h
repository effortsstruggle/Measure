#ifndef MESSAGEBOX_H
#define MESSAGEBOX_H

#include <QMouseEvent>
#include <QWidget>
#include <QDialog>
#include <QString>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QDebug>

//#include <QCloseEvent>


/* 生成参数行 */
// name:参数的名称
class createParaWid : public QWidget
{
    Q_OBJECT

public:
    createParaWid(QString name , QWidget* parent=nullptr) : QWidget(parent)
    {
        QWidget *parWid = new QWidget(this);
        QHBoxLayout *parWidLayout = new QHBoxLayout(parWid);
        parWidLayout->setContentsMargins(0,0,0,0);
        parWidLayout->setSpacing(12);
        // 参数名
        QLabel *parName = new QLabel;
        parName->setAlignment(Qt::AlignRight);
        //    parName->setAlignment(Qt::AlignVCenter);
        parName->setObjectName("msg_parName");
        parName->setText(name);
        // 输入框
        parValue = new QLineEdit;
        parValue->setObjectName("msg_parValue");
//        parValue->setMaximumWidth(200);
        parWidLayout->addWidget(parName);
        parWidLayout->setStretchFactor(parName,3);
        parWidLayout->addWidget(parValue);
        parWidLayout->setStretchFactor(parValue,10);
    }

public:
    QString getValue()
    {
        str =  parValue->text();
        return str;
    }

private:
    QLineEdit *parValue =nullptr;
    QString str;
};

/**
 * @brief The MessageBox class 弹窗
 */
class MessageBox : public QDialog
{
    Q_OBJECT
public:
    MessageBox(int type, QWidget *parent = nullptr);

    int value ;

    //    QWidget* createParaWid(QString name);

    void closeWindow();

    //    void closeEvent(QCloseEvent *event);

    /*void mousePressEvent(QMouseEvent *qevent);            //鼠标按下事件
    void mouseReleaseEvent(QMouseEvent *qevent);         //鼠标释放事件
    void mouseMoveEvent(QMouseEvent *qevent); */            //鼠标移动事件

signals:
    void clickedDetermine(QString v1,QString v2,QString v3)
        ;
protected slots:
    void btn_clicked_determine();

private:
    createParaWid *par1 =nullptr;
    createParaWid *par2 =nullptr;
    createParaWid *par3 =nullptr;
    createParaWid *par4 =nullptr;
    createParaWid *par5 =nullptr;
    createParaWid *par6 =nullptr;

    QString v1;
    QString v2;
    QString v3;
//    QString v4;
//    QString v5;
//    QString v6;
};

#endif // MESSAGEBOX_H
