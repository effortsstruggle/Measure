#ifndef SOFTMEASURE_H
#define SOFTMEASURE_H

#include <QWidget>
#include <QList>
#include <QString>
#include <QMap>
#include <QLabel>
#include <QGridLayout>
#include <QPushButton>
#include "messagebox.h"
#include "./../Common/common.h"


class Parmodel:public QWidget
{
    Q_OBJECT
public:
    Parmodel(PARMODELWIDGETFLAG index ,  QList<QList<QString>> paras,QList<QString> names,QList<QString> outs,int type,QWidget* parent=nullptr ):QWidget(parent)
    {
        this->m_index = index;
        this->setObjectName("soft_modelWid");
        QGridLayout *modelLayout = new QGridLayout(this);
        modelLayout->setContentsMargins(2,2,2,2);
        //        modelLayout->setSpacing(10);

        /****参数部分*****/
        for(int i=0;i< paras.length() ; i++ )
        {
            // 输入类型
            QLabel *type1 = new QLabel;
            type1->setMaximumHeight(34);
            type1->setText( paras[i][1] );
            type1->setAlignment(Qt::AlignCenter);
            type1->setObjectName("soft_type");

            // 参数块
            QWidget *paraValue = new QWidget;
            paraValue->setMaximumHeight(34);
            paraValue->setObjectName("soft_paraValue");
            QHBoxLayout *paraValueLayout = new QHBoxLayout(paraValue);
            paraValueLayout->setContentsMargins(2,2,2,2);
            paraValueLayout->setSpacing(0);

            // 参数名
            QLabel *parameter1 = new QLabel;
            parameter1->setObjectName("soft_parameter");
            parameter1->setText(paras[i][2]);
            parameter1->setAlignment(Qt::AlignLeft);
            parameter1->setAlignment(Qt::AlignVCenter);

            // 参数值
            value1 = new QLineEdit;
            value1->setText(paras[i][3]);
            value1->setObjectName("msg_parValue");
            value1->setMinimumWidth(90);
            value1->setAlignment(Qt::AlignLeft);
            /************进行槽连接***************/
            QObject::connect(value1,&QLineEdit::returnPressed,this,[=](){
                emit retLineText();
            });

            this->m_parasWidgets.insert(paras[i][0] + "_" + "soft_value",value1);

            // 单位
            QLabel *unit = new QLabel;
            unit->setText(paras[i][4]);
            unit->setObjectName("soft_unit");
            unit->setAlignment(Qt::AlignRight);
            unit->setAlignment(Qt::AlignBottom);

            // 布局
            paraValueLayout->addWidget(parameter1);
            paraValueLayout->setStretchFactor(parameter1,2);
            paraValueLayout->addWidget(value1);
            paraValueLayout->setStretchFactor(value1,1);
            paraValueLayout->addWidget(unit);
            paraValueLayout->setStretchFactor(unit,1);

            modelLayout->addWidget(type1,i,0,1,1);
            modelLayout->addWidget(paraValue,i,1,1,2);
        }

        /******模块名称部分*******/
        QWidget *modelName = new QWidget;
        modelName->setObjectName("soft_modelName");
        modelName->setMaximumWidth(200);
        //        modelName->setMaximumHeight(400);
        QVBoxLayout *modelNameLayout = new QVBoxLayout(modelName);
        modelNameLayout->setContentsMargins(10,0,10,0);
        modelNameLayout->setSpacing(22);
        modelNameLayout->setAlignment(Qt::AlignCenter);

        QHBoxLayout *butLayout = new QHBoxLayout;
        butLayout->setContentsMargins(0,0,0,0);
        butLayout->setSpacing(0);
        icon1 = new QPushButton;
        //        icon1->setObjectName("soft_icon_1");
        icon1->setFlat(true);
        icon1->setIcon(QIcon(":/staticFile/images/"+names[0]));
        icon1->setIconSize(QSize(47,35));
        icon1->setMaximumSize(47,35);
        icon1->setMinimumSize(47,35);
        //        connect(icon1,&QPushButton::clicked,this,&Parmodel::setParameter);
        butLayout->addWidget(icon1);

        QLabel *name1 = new QLabel;
        name1->setAlignment(Qt::AlignCenter);
        name1->setMinimumSize(44,35);
        name1->setObjectName("soft_name1");
        name1->setText( names[1] );
        // 自动换行
        name1->setWordWrap(true);

        modelNameLayout->addLayout(butLayout);
        modelNameLayout->addWidget(name1);
        m_namesWidgets.insert("soft_name1",name1);


        /*******结果部分********/
        if(type == 0)
        {
            // 计算结果部分
            QWidget *modelOut = new QWidget;
            modelOut->setObjectName("soft_modelOut");
            modelOut->setMaximumSize(220,136);
            QVBoxLayout *modelOutLayout = new QVBoxLayout(modelOut);
            modelOutLayout->setContentsMargins(0,0,0,0);
            modelOutLayout->setSpacing(0);

            QLabel *outText = new QLabel;
            outText->setObjectName("soft_outText");
            outText->setAlignment(Qt::AlignCenter);
            outText->setText(outs[0]);
            m_outsWidgets.insert("soft_modelOut",outText);

            QLabel *outText_1 = new QLabel;
            outText_1->setObjectName("soft_outText");
            outText_1->setAlignment(Qt::AlignCenter);
            outText_1->setText("阻力系数");
            m_outsWidgets.insert("soft_modelOut_1",outText_1);


            // 输出值与单位部分
            QHBoxLayout *valueLayout = new QHBoxLayout;
            valueLayout->setContentsMargins(0,0,0,0);
            valueLayout->setSpacing(0);

            QLabel *outValue = new QLabel;
            outValue->setObjectName("soft_outValue");
            outValue->setText( outs[1] );
            outValue->setAlignment(Qt::AlignRight);
            m_outsWidgets.insert("soft_outValue",outValue);

            QLabel *outValue_1 = new QLabel;
            outValue_1->setObjectName("soft_outValue");
            outValue_1->setText( outs[1] );
            outValue_1->setAlignment(Qt::AlignCenter);
            m_outsWidgets.insert("zlxs_soft_value",outValue_1);

            QLabel *outUnit = new QLabel;
            outUnit->setObjectName("soft_outUnit");
            outUnit->setText( outs[2] );
            outUnit->setAlignment(Qt::AlignLeft);
            outUnit->setAlignment(Qt::AlignBottom);
            valueLayout->addWidget(outValue);
            valueLayout->addWidget(outUnit);
            m_outsWidgets.insert("soft_outUnit",outUnit);


            modelOutLayout->addWidget(outText);
            modelOutLayout->setStretchFactor(outText,2);
            modelOutLayout->addLayout(valueLayout);
            modelOutLayout->setStretchFactor(valueLayout,1);
            modelOutLayout->addWidget(outText_1);
            modelOutLayout->setStretchFactor(outText_1,2);
            modelOutLayout->addWidget(outValue_1);
            modelLayout->addWidget(modelName,0,3,paras.length(),1);
            modelLayout->addWidget(modelOut,0,4,paras.length(),1);
        }
        else
        {
            // 计算结果部分
            QWidget *modelOut = new QWidget;
            modelOut->setObjectName("soft_modelOut");
            modelOut->setMaximumSize(110,68);
            QVBoxLayout *modelOutLayout = new QVBoxLayout(modelOut);
            modelOutLayout->setContentsMargins(0,0,0,0);
            modelOutLayout->setSpacing(0);

            QLabel *outText = new QLabel;
            outText->setObjectName("soft_outText");
            outText->setAlignment(Qt::AlignCenter);
            outText->setText( outs[0] );
            this->m_outsWidgets.insert("soft_modelOut",outText);

            // 输出值与单位部分
            QHBoxLayout *valueLayout = new QHBoxLayout;
            valueLayout->setContentsMargins(0,0,0,0);
            valueLayout->setSpacing(0);

            QLabel *outValue = new QLabel;
            outValue->setObjectName("soft_outValue");
            outValue->setText( outs[1] );
            outValue->setAlignment(Qt::AlignLeft);
            this->m_outsWidgets.insert("soft_outValue",outValue);

            QLabel *outUnit = new QLabel;
            outUnit->setObjectName("soft_outUnit");
            outUnit->setText( outs[2] );
            outUnit->setAlignment(Qt::AlignLeft);
            outUnit->setAlignment(Qt::AlignBottom);
            valueLayout->addWidget(outValue);
            valueLayout->addWidget(outUnit);
            this->m_outsWidgets.insert("soft_outUnit",outUnit);


            modelOutLayout->addWidget(outText);
            modelOutLayout->setStretchFactor(outText,2);
            modelOutLayout->addLayout(valueLayout);
            modelOutLayout->setStretchFactor(valueLayout,1);
            modelLayout->addWidget(modelName,0,3,paras.length(),1);
            modelLayout->addWidget(modelOut,0,4,paras.length(),1);
        }
    }

    void setParasValue(QString title,QString value)
    {
        QLineEdit* pLineEdit = this->m_parasWidgets[title];
        if(pLineEdit == nullptr)
        {
            return;
        }
        pLineEdit->setText(value);
    }

    QString getParasValue(QString title)
    {
        QLineEdit* pLabel = this->m_parasWidgets[title];

        if(pLabel == nullptr)
        {
            return NULL;
        }
        QString value = pLabel->text();
        return value;
    }

    /**
     * @brief setresultValue 设置计算的结果值
     * @param title
     * @param value
     */
    void setResultValue(QString title,QString value)
    {
        QLabel* pLabel = this->m_outsWidgets[title];
        if(pLabel == nullptr)
        {
            return;
        }
        pLabel->setText(value);
    }

    /**
     * @brief getResultValue 获取计算结果值
     * @param title
     * @return
     */
    float getResultValue( QString title )
    {
        QLabel* pLabel = this->m_outsWidgets[title];
        if(pLabel == nullptr)
        {
            return 0.0 ;
        }
        return pLabel->text().toFloat();
    }


signals:
    void setvalue( PARMODELWIDGETFLAG index ,  QString v1,QString v2,QString v3 );

    void retLineText();

public slots:
    /* 点击图标弹窗设置参数值 */
    void setParameter()
    {

        MessageBox *msgBox = new MessageBox(0);
        //        msgBox->showFullScreen();
        QObject::connect( msgBox , &MessageBox::clickedDetermine , [=](QString v1,QString v2,QString v3){
            emit setvalue(this->m_index , v1,v2,v3);
        });

        msgBox->exec();

        return;
    }

    void setParameter_1(){

        MessageBox *msgBox = new MessageBox(1);
        //        msgBox->showFullScreen();
        QObject::connect(msgBox,&MessageBox::clickedDetermine,[=](QString v1,QString v2,QString v3){
            emit setvalue(this->m_index , v1 , v2 , v3);
        });

        msgBox->exec();

        return;
    }

    void setParameter_2(){

        MessageBox *msgBox = new MessageBox(2);
        //        msgBox->showFullScreen();
        QObject::connect(msgBox,&MessageBox::clickedDetermine,[=](QString v1,QString v2,QString v3){
            emit setvalue(this->m_index , v1 , v2 , v3);
        });

        msgBox->exec();

        return;
    }

    void setParameter_3(){

        MessageBox *msgBox = new MessageBox(3);
        //        msgBox->showFullScreen();
        QObject::connect(msgBox,&MessageBox::clickedDetermine,[=](QString v1,QString v2,QString v3){
            emit setvalue(this->m_index , v1 , v2 , v3);
        });

        msgBox->exec();

        return;
    }

public:
    QPushButton* icon1;
    QLineEdit *value1;

private:
    PARMODELWIDGETFLAG m_index;
    QHash<QString,QLineEdit*> m_parasWidgets;
    QHash<QString,QLabel*> m_namesWidgets;
    QHash<QString,QLabel*> m_outsWidgets;
};


/**
 * @brief The SoftMeasureWidget class 软测量监控页面
 */
class ModbusData;
class SoftMeasureWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SoftMeasureWidget(QString zlsjlfName , QString flsjlfName , QString cyqyltfgName ,
                         QString lsbName_1 , QString lsbName_2 , QString lsbqljpqfName_1 ,
                         QString lsbqljpqfName_2 , QString lsgName , QWidget *parent = nullptr);

    void initGUI();

    void initConnect();

    void updataGUI( PARMODELWIDGETFLAG index , double value_1 , double value_2 , double value_3  );

    void setResultValue(  PARMODELWIDGETFLAG index_ , QString v1,QString v2,QString v3 );
    float getResultValue( PARMODELWIDGETFLAG  index_ );


    void setParasValue_zlsjltjf(QString title , QString value);
signals:
//    void getSBD(QString name , double value );

private:
    QHash< PARMODELWIDGETFLAG , Parmodel *> m_parmodelList;
    QString m_zlsjlfName;
    QString m_flsjlfName;
    QString m_cyqyltjfName;
    QString m_lsbName_1;
    QString m_lsbName_2;
    QString m_lsbqljpqfName_1;
    QString m_lsbqljpqfName_2;
    QString m_lsgName;
};

#endif // SOFTMEASURE_H
