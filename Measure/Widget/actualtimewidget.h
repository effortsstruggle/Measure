#ifndef ACTUALTIMEWIDGET_H
#define ACTUALTIMEWIDGET_H

#include <QWidget>
#include <QString>
//#include <QtCharts/QChartGlobal>
//#include <QtCharts/QChart>
//#include <QLineSeries>
//#include <QtCharts/QLineSeries>
#include <QPushButton>
#include <QTcpSocket>
#include <QTcpServer>
#include <QByteArray>
#include <QChar>
#include <QTimer>
#include <QLabel>
#include <QHBoxLayout>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include "softmeasurewidget.h"


class ParWidget:public QWidget
{
public:
    ParWidget(QString name,QString value,QString unit,QWidget* parent = nullptr):QWidget(parent)
    {
        /* 参数行 */
        //        QWidget *parameterWid = new QWidget(this);
        this->setObjectName("actual_parameterWid");
        this->setMaximumWidth(200);
        QHBoxLayout *parWidLayout = new QHBoxLayout( this );
        parWidLayout->setContentsMargins(10,10,10,0);
        parWidLayout->setSpacing(0);

        // 参数名
        parName = new QLabel;
        parName->setAlignment(Qt::AlignVCenter);
        parName->setObjectName("actual_parName");
        parName->setText(name);

        // 参数值
        parValue = new QLabel;
        parValue->setAlignment(Qt::AlignLeft);
//        parValue->setAlignment(Qt::AlignHCenter);
        parValue->setObjectName("actual_parValue");
        parValue->setText(value);

        // 参数单位
        parUnit = new QLabel;
        //    parUnit->setAlignment(Qt::AlignLeft);
        parUnit->setAlignment(Qt::AlignBottom);
        parUnit->setObjectName("actual_parUnit");
        parUnit->setText(unit);

        parWidLayout->addWidget(parName);
        parWidLayout->setStretchFactor(parName,2);
        parWidLayout->addWidget(parValue);
        parWidLayout->setStretchFactor(parValue,3);
        parWidLayout->addWidget(parUnit);
        parWidLayout->setStretchFactor(parUnit,1);

    }
    void setValue(QString text)
    {
        this->parValue->setText(text);
    }

private:
    QLabel *parName = nullptr;
    QLabel *parValue = nullptr;
    QLabel *parUnit = nullptr;
};

/**
 * @brief The ActualTime class 实时监控页面
 */
class ActualTimeWidget:public QWidget
{
    Q_OBJECT
public:
    explicit ActualTimeWidget(QString zlsqjlf_name , QString flsqjlf_name ,
                              QString cyqyltjf_name , QString qljpqf_name_1 ,
                              QString qljpqf_name_2 , QString lsbqlj_name_1 ,
                              QString lsbqlj_name_2 , QString lsgzlxs ,
                              QWidget *parent = nullptr);

    QWidget* CreateParWid(QString name,QString value,QString unit);
    void initGUI();

    void ControlButClick(QString name);
    //    void init_sql();
    //    void Find_database();
    //    void database_insert();
    //    void setvalue(QString name,double value);
    void updateGUI(PARMODELWIDGETFLAG index , float value_1 );

    //protected:
    //    virtual void resizeEvent(QResizeEvent *event) override;

signals:
    void updatavalue(QMap<QString,QString>map);

private:

    //    ParWidget *parameter1Wid = nullptr;
    //    ParWidget *parameter2Wid;
    ParWidget *parameterZlsqjlf;
    ParWidget *parameterFlsqjlf;
    ParWidget *parameterLsbpqf_1;
    ParWidget *parameterLsbpqf_2;
    ParWidget *parameterCyqyltjf;
    ParWidget *parameterLsbqlj_1;
    ParWidget *parameterLsbqlj_2;
    ParWidget *parameterLsgzlxs;




    QString m_zlsqjlfName;
    QString m_flsqjlfName;
    QString m_cyqyltjfName;
    QString m_qljpqfName_1;
    QString m_qljpqfName_2;
    QString m_lsbqljName_1;
    QString m_lsbqljName_2;
    QString m_lsgzlxsName;

    //    ParWidget *parameter9Wid = nullptr;
    //    ParWidget *parameter10Wid = nullptr;
    //    ParWidget *parameter11Wid = nullptr;
    //    ParWidget *parameter12Wid = nullptr;
    //    ParWidget *parameter13Wid = nullptr;
    //    ParWidget *parameter14Wid = nullptr;
    //    ParWidget *parameter15Wid = nullptr;
    //    ParWidget *parameter16Wid = nullptr;
    //    ParWidget *parameter17Wid = nullptr;
    //    ParWidget *parameter18Wid = nullptr;
    //    ParWidget *parameter19Wid = nullptr;
    //    ParWidget *parameter20Wid = nullptr;

};

#endif // ACTUALTIMEWIDGET_H
