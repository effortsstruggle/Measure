#include "messagebox.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QGridLayout>

MessageBox::MessageBox(int type,QWidget *parent):QDialog(parent)
{

    /* 消息弹窗 */
    this->resize(700,360);
    // 不显示标题栏（Qt无法自定义标题栏，标题栏定义需要用到操作系统的api）
    this->setWindowFlags(Qt::FramelessWindowHint);

    //messageBox主窗体
    QWidget *mainWid = new QWidget(this);
    mainWid->setObjectName("msg_mainWid");
    mainWid->resize(700,360);
    QVBoxLayout *mainWidLayout = new QVBoxLayout(mainWid);
    mainWidLayout->setContentsMargins(0,0,0,0);
    mainWidLayout->setSpacing(0);


    /* 标题栏 */
    QWidget *titleWid = new QWidget;
    titleWid->setMaximumHeight(50);
    titleWid->setObjectName("msg_titleWid");
    QHBoxLayout *titleLayout = new QHBoxLayout(titleWid);
    titleLayout->setContentsMargins(0,0,0,0);
    titleLayout->setSpacing(0);
    // 左侧窗口名称
    QLabel *title = new QLabel;
    title->setObjectName("msg_title");
    title->setText("参数设置");
    title->setAlignment(Qt::AlignLeft);

    // 右侧关闭按钮
    QPushButton *closeBut = new QPushButton;
    closeBut->setObjectName("msg_closeBut");
    closeBut->setIcon(QIcon(":/staticFile/images/icon_bg.png"));
    closeBut->setIconSize(QSize(40,40));
    connect(closeBut,&QPushButton::clicked,this,&MessageBox::closeWindow);
    // 标题栏布局
    titleLayout->addWidget(title);
    titleLayout->setStretchFactor(title,10);
    titleLayout->addWidget(closeBut);
    titleLayout->setStretchFactor(closeBut,1);



    /* 参数块 */
    QGridLayout *parLayout = new QGridLayout;
    parLayout->setContentsMargins(39,30,55,30);
    parLayout->setSpacing(0);

    if(type == 0)
    {
        par1 = new createParaWid("密度");
        par2 = new createParaWid("温度");
        parLayout->addWidget(par1,0,0);
        parLayout->addWidget(par2,0,1);
    }
    else if(type ==1)
    {
        par1 = new createParaWid("扬程");
        parLayout->addWidget(par1,0,0);
    }
    else if(type ==2)
    {
        par1 =new createParaWid("温度");
        parLayout->addWidget(par1,0,0);
    }
    else
    {
        par1 =new createParaWid("");
        QWidget* _winwod = new QWidget(this);
        _winwod->setMinimumHeight(50);
        _winwod->setMinimumWidth(200);
        _winwod->setWindowOpacity(0);
        parLayout->addWidget(_winwod,0,0);
    }


//    parLayout->addWidget(par3,1,0);
//    parLayout->addWidget(par4,1,1);
//    parLayout->addWidget(par5,2,0);
//    parLayout->addWidget(par6,2,1);


    /* 按钮组 */
    QWidget *butGroup = new QWidget;
    butGroup->setMaximumHeight(60);
    butGroup->setObjectName("msg_butGroup");
    QHBoxLayout *butGroupLayout= new QHBoxLayout(butGroup);
    butGroupLayout->setContentsMargins(260,0,260,0);
    butGroupLayout->setSpacing(16);
    QPushButton *defineBut = new QPushButton;
    defineBut->setText("确定");
    connect(defineBut,&QPushButton::clicked,this,&MessageBox::btn_clicked_determine);
    defineBut->setObjectName("msg_defineBut");
    QPushButton *cancelBut = new QPushButton;
    cancelBut->setText("取消");
    connect(cancelBut,&QPushButton::clicked,this,&MessageBox::closeWindow);
    cancelBut->setObjectName("msg_cancelBut");
    butGroupLayout->addWidget(cancelBut);
    butGroupLayout->addWidget(defineBut);

    mainWidLayout->addWidget(titleWid);
    mainWidLayout->addLayout(parLayout);
    mainWidLayout->addWidget(butGroup);

    value = type;

}

/* 生成参数行 */
// name:参数的名称
//QWidget* MessageBox::createParaWid(QString name){
//    QWidget *parWid = new QWidget;
//    QHBoxLayout *parWidLayout = new QHBoxLayout(parWid);
//    parWidLayout->setContentsMargins(0,0,0,0);
//    parWidLayout->setSpacing(12);
//    // 参数名
//    QLabel *parName = new QLabel;
//    parName->setAlignment(Qt::AlignRight);
//    parName->setAlignment(Qt::AlignVCenter);
//    parName->setObjectName("msg_parName");
//    parName->setText(name);
//    // 输入框
//    QLineEdit *parValue = new QLineEdit;
//    parValue->setObjectName("msg_parValue");
//    parValue->setMaximumWidth(200);
//    parWidLayout->addWidget(parName);
//    parWidLayout->setStretchFactor(parName,3);
//    parWidLayout->addWidget(parValue);
//    parWidLayout->setStretchFactor(parValue,10);

//    return parWid;
//}

/* 窗口关闭事件 */
void MessageBox::closeWindow(){

    close();

    return;
}

void MessageBox::btn_clicked_determine()
{
    this->close();

    if(value == 0)
    {
      v1 = par1->getValue();
      v2 = par2->getValue();
      v3 = "";
    }
    else if(value == 1)
    {
      v1 = "";
      v2 = "";
      v3 =par1->getValue();
    }
    else if(value == 2)
    {
      v1 =par1->getValue();
      v2 = "";
      v3 = "";
    }
    else
    {
       v1 = "";
       v2 = "";
       v3 = "";
    }

//    v4 = par4->getValue();
//    v5 = par5->getValue();
//    v6 = par6->getValue();

    emit clickedDetermine(v1,v2,v3);
}

// /* 窗口关闭事件 */
//void MessageBox::closeEvent(QCloseEvent *event){


//}
