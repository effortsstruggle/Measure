#include "actualtimewidget.h"
#include <QCoreApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QLabel>
//#include <QChart>
//#include <QChartView>
//#include <QLineSeries>
#include <QPushButton>
#include <QStyle>
#include <QComboBox>
#include <QDebug>
#include <QIODevice>
#include <QString>
#include "./Data/modbusdata.h"

//using namespace QtCharts;

ActualTimeWidget::ActualTimeWidget(QString zlsqjlf_name , QString flsqjlf_name ,
                                   QString cyqyltjf_name , QString qljpqf_name_1 ,
                                   QString qljpqf_name_2 , QString lsbqlj_name_1 ,
                                   QString lsbqlj_name_2 , QString lsgzlxs_name ,
                                   QWidget *parent) : QWidget(parent)
{
    this->m_zlsqjlfName = zlsqjlf_name;
    this->m_flsqjlfName = flsqjlf_name;
    this->m_cyqyltjfName = cyqyltjf_name;
    this->m_qljpqfName_1 = qljpqf_name_1;
    this->m_qljpqfName_2 = qljpqf_name_2;
    this->m_lsbqljName_1 = lsbqlj_name_1;
    this->m_lsbqljName_2 = lsbqlj_name_2;
    this->m_lsgzlxsName = lsgzlxs_name;
    this->initGUI();
}
/* 页面初始化 */
void ActualTimeWidget::initGUI(){

    this->setContentsMargins(0,0,0,0);


    /********************主凝气器***********************/
    // 数据组1
    //    parameter1Wid = new ParWidget("VP","0","%",this);
    //    parameter1Wid->setGeometry(680,8,140,34);


    // 圆柱图
    QLabel *imgColumn = new QLabel(this);
    imgColumn->setPixmap(QPixmap(":/staticFile/images/img_03.png"));
    imgColumn->setGeometry(1130+250,12,150,286);
    imgColumn->raise();

    //主凝气器
    QLabel *condenser = new QLabel(this);
    condenser->setPixmap(QPixmap(":/staticFile/images/img_01.png"));
    condenser->setGeometry(81+247,76,190,110);
    QLabel *condenserText = new QLabel(this);
    condenserText->setText("主凝汽器");
    condenserText->setObjectName("actual_condenserText");
    condenserText->setGeometry(135+250,149,81,21);
    // 短线路
    QLabel *shutline = new QLabel(this);
    shutline->setPixmap(QPixmap(":/staticFile/images/img_05.png"));
    shutline->setGeometry(804+250,82,328,20);

    // 线路
    QLabel *pipeline = new QLabel(this);
    pipeline->setPixmap(QPixmap(":/staticFile/images/img_04_new_2.png"));
    pipeline->setGeometry(171+250,145,964,371);
    pipeline->lower();

    //主凝水器节流阀
    QLabel *label_zlsqjlfText = new QLabel(this);
    label_zlsqjlfText->setObjectName("actual_label_zlsqjlfText");
    label_zlsqjlfText->setText(this->m_zlsqjlfName);
    label_zlsqjlfText->setGeometry(826+230,203-70,140,100);
    QLabel *label_zlsqjlfIcon = new QLabel(this);
    label_zlsqjlfIcon->setPixmap(QPixmap(":/staticFile/images/img_06.png"));
    label_zlsqjlfIcon->setGeometry(826+250,203,74,55);
    this->parameterZlsqjlf = new ParWidget("Q","0","t/h",this);
    this->parameterZlsqjlf->setGeometry(790+270,281-20,130,34);



    //凝水泵汽轮机配气阀-1
    QLabel *label_lsbpqfText1 = new QLabel(this);
    label_lsbpqfText1->setObjectName("actual_label_lsbpqfText1");
    label_lsbpqfText1->setText(this->m_qljpqfName_1);
    label_lsbpqfText1->setGeometry(371+165,230-90,128,100);
    QLabel *label_lsbpqfIcon1 = new QLabel(this);
    label_lsbpqfIcon1->setPixmap(QPixmap(":/staticFile/images/img_07.png"));
    label_lsbpqfIcon1->setGeometry(371+165,230-25,74,55);
    this->parameterLsbpqf_1 = new ParWidget("Q","0","t/h",this);
    this->parameterLsbpqf_1->setGeometry(371+165-20,230+35,130,34);



    //凝水泵汽轮机配气阀-2
    QLabel *label_lsbpqfText2 = new QLabel(this);
    label_lsbpqfText2->setObjectName("actual_label_lsbpqfText2");
    label_lsbpqfText2->setText(this->m_qljpqfName_2);
    label_lsbpqfText2->setGeometry(371+165,230+185-90,128,100);
    QLabel *label_lsbpqfIcon2 = new QLabel(this);
    label_lsbpqfIcon2->setPixmap(QPixmap(":/staticFile/images/img_07.png"));
    label_lsbpqfIcon2->setGeometry(371+165,230+185-25,74,55);
    this->parameterLsbpqf_2 = new ParWidget("Q","0","t/h",this);
    this->parameterLsbpqf_2->setGeometry(371+165-20, 230+185+35 , 130 , 34 );


    //凝水泵汽轮机-1
    QLabel *label_lsbText1 = new QLabel(this);
    label_lsbText1->setObjectName("actual_label_lsbText1");
    label_lsbText1->setText(this->m_lsbqljName_1);
    label_lsbText1->setGeometry(371+304,160,128,100);
    QLabel *label_lsbIcon1 = new QLabel(this);
    label_lsbIcon1->setPixmap(QPixmap(":/staticFile/images/img_02.png"));
    label_lsbIcon1->setGeometry(371+284,230,128,100);
    this->parameterLsbqlj_1 = new ParWidget("Q","0","m3/h",this);
    this->parameterLsbqlj_1->setGeometry(363+284+130,345-50,130,34);

    //凝水泵汽轮机-2
    QLabel *label_lsbText2 = new QLabel(this);
    label_lsbText2->setObjectName("actual_label_lsbText2");
    label_lsbText2->setText(this->m_lsbqljName_2);
    label_lsbText2->setGeometry(371+304,230+115,128,100);
    QLabel *label_lsbIcon2 = new QLabel(this);
    label_lsbIcon2->setPixmap(QPixmap(":/staticFile/images/img_02.png"));
    label_lsbIcon2->setGeometry(371+284,230+185,128,100);
    this->parameterLsbqlj_2 = new ParWidget("Q","0","m3/h",this);
    this->parameterLsbqlj_2->setGeometry(363+284+130,345+185-50,130,34);



    // 开关1
    QLabel *IO1 = new QLabel(this);
    IO1->setPixmap(QPixmap(":/staticFile/images/img_06.png"));
    IO1->setGeometry(563+250,118,74,55);
    //    this->parameter2Wid = new ParWidget("Q","3.23","m3/h",this);
    //    this->parameter2Wid->setGeometry(530+270,45+30+105,100,34);

    // 开关2
    QLabel *label_cyqyltjfText = new QLabel(this);
    label_cyqyltjfText->setObjectName("actual_label_cyqyltjfText");
    label_cyqyltjfText->setText(this->m_cyqyltjfName);
    label_cyqyltjfText->setGeometry(966+190,-10,200,100);
    QLabel *label_cyqyltjfIcon= new QLabel(this);
    label_cyqyltjfIcon->setPixmap(QPixmap(":/staticFile/images/img_07.png"));
    label_cyqyltjfIcon->setGeometry(966+250,57,74,55);
    this->parameterCyqyltjf = new ParWidget("Q","45","t/h",this);
    this->parameterCyqyltjf->setGeometry(940+265,8+5+105,130,34);






    //凝水管阻力系数
    QLabel *label_lsgzlxs = new QLabel(this);
    label_lsgzlxs->setObjectName("actual_labelLsgzlxs");
    label_lsgzlxs->setText(this->m_lsgzlxsName);
    label_lsgzlxs->setGeometry(826+250+160 ,203+100,140,55);
    this->parameterLsgzlxs = new ParWidget("","0","",this);
    this->parameterLsgzlxs->setGeometry(790+270+190,281+65,130,34);


    // 参数组3
    //    parameter8Wid = new ParWidget("VP","0","%");
    //    parameter8Wid->setGeometry(790,273,140,34);
    //    parameter9Wid = new ParWidget("Dp","0","kPa");
    //    parameter9Wid->setGeometry(790,317,140,34);

    //    this->parameter10Wid = new ParWidget("Q","0","t/h");
    //    this->parameter10Wid->setGeometry(790+250,361,140,34);

    //    // 下拉选择框1
    //    QComboBox *options1 = new QComboBox(this);
    //    options1->setGeometry(1712,19,140,34);
    //    options1->setObjectName("actual_options");
    //    //添加元素
    //    options1->addItem("VP");
    //    options1->addItem("Dp");
    //    options1->addItem("Q");
    //    //设置默认选中
    //    options1->setCurrentIndex(0);
    //    options1->model()->setData(options1->model()->index(0,0),QSize(140,34),Qt::SizeHintRole);
    //    options1->model()->setData(options1->model()->index(1,0),QSize(140,34),Qt::SizeHintRole);
    //    options1->model()->setData(options1->model()->index(2,0),QSize(140,34),Qt::SizeHintRole);
    //    options1->raise();

    //    /* 右侧折线图1 */
    //    QLineSeries *series = new QLineSeries();
    //    series->setColor(QColor("#51CBF9"));
    //    //    series->setPen(QPen(QBrush(Qt::red),2,Qt::DashLine));
    //    //int vp =(modbustcp->m_data2.value(modbustcp->data2::X_k)).toInt();
    //    *series << QPointF(5, 25)
    //            << QPointF(15, 7)
    //            << QPointF(19, 30)
    //            << QPointF(21, 21)
    //            << QPointF(23, 24)
    //            << QPointF(25, 21)
    //            << QPointF(28, 30)
    //            << QPointF(32, 14);
    //    QChart *chart = new QChart();

    //    // 设置背景透明
    //    chart->setBackgroundVisible(false);
    //    chart->legend()->hide();
    //    chart->addSeries(series);
    //    chart->createDefaultAxes();
    //    // 坐标轴上字的颜色
    //    chart->axisX()->setLabelsColor(QColor("#38608b"));
    //    chart->axisY()->setLabelsColor(QColor("#38608b"));

    //    QChartView *chartView = new QChartView(this);
    //    chartView->setStyleSheet("background:transparent");
    //    chartView->setChart(chart);
    //    chartView->setRenderHint(QPainter::Antialiasing);
    //    chartView->setGeometry(1353,19,521,343);
    //    chartView->lower();



    /********************辅凝气器***********************/
    /* 第二组数据展示 */
    // 数据组4
    //    parameter11Wid = new ParWidget("VP","0","%",this);
    //    parameter11Wid->setGeometry(680,483,140,34);
    //        this->parameter12Wid = new ParWidget("阻力系数","0","",this);
    //        this->parameter12Wid->setGeometry(830 - 34,483,140,34);
    //        this->parameter13Wid = new ParWidget("阻力","0","kPa",this);
    //        this->parameter13Wid->setGeometry(940,483,140,34);
    // 圆柱图2
    //        QLabel *imgColumn2 = new QLabel(this);
    //        imgColumn2->raise();
    //        imgColumn2->setPixmap(QPixmap(":/staticFile/images/img_03.png"));
    //        imgColumn2->setGeometry(1130,487,150,286);

    QLabel *condenser2 = new QLabel(this);
    condenser2->setPixmap(QPixmap(":/staticFile/images/img_01.png"));
    condenser2->setGeometry(81+250,554,190,110);
    QLabel *condenserText2 = new QLabel(this);
    condenserText2->setText("辅凝汽器");
    condenserText2->setObjectName("actual_condenserText");
    condenserText2->setGeometry(135+250,624,81,21);
    //        // 短线路2
    //        QLabel *shutline2 = new QLabel(this);
    //        shutline2->setPixmap(QPixmap(":/staticFile/images/img_05.png"));
    //        shutline2->setGeometry(804 - 26,557,328,20);
    //        // 开关4
    //        QLabel *IO4 = new QLabel(this);
    //        IO4->setPixmap(QPixmap(":/staticFile/images/img_06.png"));
    //        IO4->setGeometry(940+250,532,74,55);
    // 线路2
    QLabel *pipeline2 = new QLabel(this);
    pipeline2->setPixmap(QPixmap(":/staticFile/images/img_04_old.png"));
    pipeline2->setGeometry(171+250,620,964,185);
    pipeline2->lower();
    // 输风转子2
    QLabel *wind2 = new QLabel(this);
    wind2->setPixmap(QPixmap(":/staticFile/images/img_02.png"));
    wind2->setGeometry(371+250,705,128,100);
    // 开关7
    QLabel *IO7 = new QLabel(this);
    IO7->setPixmap(QPixmap(":/staticFile/images/img_06.png"));
    IO7->setGeometry(563+250,593,74,55);


    //辅凝水器节流阀
    QLabel *label_flsqjlfText = new QLabel(this);
    label_flsqjlfText->setObjectName("actual_label_flsqjlfText");
    label_flsqjlfText->setText(this->m_flsqjlfName);
    label_flsqjlfText->setGeometry(826+230,678-70,140,100);
    QLabel *label_flsqjlfIcon = new QLabel(this);
    label_flsqjlfIcon->setPixmap(QPixmap(":/staticFile/images/img_06.png"));
    label_flsqjlfIcon->setGeometry(826+250,678,74,55);
    this->parameterFlsqjlf = new ParWidget("Q","0","t/h",this);
    this->parameterFlsqjlf->setGeometry(790+270,678+60,130,34);

    // 参数14
    //    parameter14Wid = new ParWidget("Dp","0","kPa",this);
    //    parameter14Wid->setGeometry(955,656,140,34);
    // 参数组2
    //    parameter15Wid = new ParWidget("","0","r/min",this);
    //    parameter15Wid->setGeometry(363,820,140,34);
    //    parameter16Wid = new ParWidget("Dp","0","",this);
    //    parameter16Wid->setGeometry(363,864,140,34);
    //        this->parameter17Wid = new ParWidget("汽轮机","0","t/h",this);
    //        this->parameter17Wid->setGeometry(363,820,140,34);
    // 参数组3
    //    parameter18Wid = new ParWidget("VP","0","%",this);
    //    parameter18Wid->setGeometry(790,748,140,34);
    //    parameter19Wid = new ParWidget("Dp","0","kPa",this);
    //    parameter19Wid->setGeometry(790,792,140,34);
    //        this->parameter20Wid = new ParWidget("Q","32","t/h",this);
    //        this->parameter20Wid->setGeometry(790,748,140,34);

    //    // 右侧折线图2
    //    QLineSeries *series2 = new QLineSeries();
    //    *series2 << QPointF(4, 11)
    //             << QPointF(5, 3)
    //             << QPointF(8, 9)
    //             << QPointF(10, 3)
    //             << QPointF(16, 13)
    //             << QPointF(16, 13)
    //             << QPointF(18, 23)
    //             << QPointF(22, 19)
    //             << QPointF(26, 13);
    //    QChart *chart2 = new QChart();
    //    chart2->setBackgroundVisible(false);
    //    chart2->legend()->hide();
    //    chart2->addSeries(series2);
    //    chart2->createDefaultAxes();
    //    // 坐标轴上字的颜色
    //    chart2->axisX()->setLabelsColor(QColor("#38608b"));
    //    chart2->axisY()->setLabelsColor(QColor("#38608b"));
    //    //    chart->setTitle("Simple line chart example");
    //    QChartView *chartView2 = new QChartView(this);
    //    chartView2->setStyleSheet("background:transparent;");
    //    chartView2->setChart(chart2);
    //    chartView2->setRenderHint(QPainter::Antialiasing);
    //    chartView2->setGeometry(1353,531,521,343);
    //    chartView2->lower();

    //    // 下拉选择框2
    //    QComboBox *options2 = new QComboBox(this);
    //    options2->setGeometry(1712,531,140,34);
    //    options2->setObjectName("actual_options");
    //    //添加元素
    //    options2->addItem("VP");
    //    options2->addItem("Dp");
    //    options2->addItem("Q");
    //    //设置默认选中
    //    options2->setCurrentIndex(1);
    //    options2->model()->setData(options2->model()->index(0,0),QSize(140,34),Qt::SizeHintRole);
    //    options2->model()->setData(options2->model()->index(1,0),QSize(140,34),Qt::SizeHintRole);
    //    options2->model()->setData(options2->model()->index(2,0),QSize(140,34),Qt::SizeHintRole);
    //    options2->raise();
    return;
}


/* 生成参数行 */
QWidget* ActualTimeWidget::CreateParWid(QString name,QString value,QString unit){
    /* 参数行 */
    QWidget *parameterWid = new QWidget(this);
    parameterWid->setObjectName("actual_parameterWid");
    parameterWid->setMaximumWidth(140);
    QHBoxLayout *parWidLayout = new QHBoxLayout(parameterWid);
    parWidLayout->setContentsMargins(10,10,10,0);
    parWidLayout->setSpacing(0);

    // 参数名
    QLabel *parName = new QLabel;
    parName->setAlignment(Qt::AlignVCenter);
    parName->setObjectName("actual_parName");
    parName->setText(name);

    // 参数值
    QLabel *parValue = new QLabel;
    parValue->setAlignment(Qt::AlignVCenter);
    parValue->setAlignment(Qt::AlignRight);
    parValue->setObjectName("actual_parValue");
    parValue->setText(value);

    // 参数单位
    QLabel *parUnit = new QLabel;
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

    return parameterWid;

}

//void ActualTimeWidget::init_sql()
//{
//    QSqlDatabase qdb = QSqlDatabase::addDatabase("QMYSQL", "UI");
//    qdb.setHostName("127.0.0.1");
//    qdb.setPort(3306);
//    qdb.setDatabaseName("tcpmodbus");
//    qdb.setUserName("root");
//    qdb.setPassword("123456");
//    if(qdb.open())
//    {
//        if(!(qdb.tables().contains("advance"))){

//            QVector <QString> bh ={"key1","id","nullp"};
//            QSqlQuery query(QSqlDatabase::database("UI"));
//            QString sql = QString("create table advance(%1 varchar(255),%2 varchar(255),%3 int)").arg(bh[0]).arg(bh[1]).arg(bh[2]);
//            bool ret1 = query.exec(sql);
//            if (!ret1) {
//                qDebug() << query.lastError();
//                qDebug() << query.lastQuery();
//            };

//        }
//    }
//    QSqlDatabase::database("UI").close();
//}

//void ActualTimeWidget::Find_database()
//{
//    QSqlDatabase qdb = QSqlDatabase::addDatabase("QMYSQL", "AI");
//    qdb.setHostName("127.0.0.1");
//    qdb.setPort(3306);
//    qdb.setDatabaseName("tcpmodbus");
//    qdb.setUserName("root");
//    qdb.setPassword("123456");
//    if(qdb.open()){
//        QSqlQuery query(QSqlDatabase::database("AI"));
//        QString sql = QString("select key1,id from advance order by X_k limit 8");
//        qDebug() << query.prepare(sql);
//        if(query.exec())
//        {
//            query.next();
//            qDebug() << query.value(0).toString();//key1
//        }
//    }
//    QSqlDatabase::database("AI").close();
//}

//void ActualTimeWidget::database_insert()
//{
//    QSqlDatabase qdb = QSqlDatabase::addDatabase("QMYSQL", "AI");
//    qdb.setHostName("127.0.0.1");
//    qdb.setPort(3306);
//    qdb.setDatabaseName("tcpmodbus");
//    qdb.setUserName("root");
//    qdb.setPassword("123456");
//    if (qdb.open()){
//        QVariantList keys;
//        QVariantList id;

//        keys.reserve( this->m_modbusTcp->getData2Count() );
//        id.reserve( this->m_modbusTcp->getData2Count() );

//        keys << this->m_modbusTcp->getData2Value(ModbusTcp::DATA2::B_tmp);
//        keys << this->m_modbusTcp->getData2Value(ModbusTcp::DATA2::B_status);
//        keys << this->m_modbusTcp->getData2Value(ModbusTcp::DATA2::X_k);
//        keys << this->m_modbusTcp->getData2Value(ModbusTcp::DATA2::X_yali);
//        keys << this->m_modbusTcp->getData2Value(ModbusTcp::DATA2::X_ll1);
//        keys << this->m_modbusTcp->getData2Value(ModbusTcp::DATA2::X_ll2);
//        keys << this->m_modbusTcp->getData2Value(ModbusTcp::DATA2::X_w);
//        keys << this->m_modbusTcp->getData2Value(ModbusTcp::DATA2::X_x);

//        id << "B_tmp";
//        id << "B_status";
//        id << "X_k";
//        id << "X_yali";
//        id << "X_ll1";
//        id << "X_ll2";
//        id << "X_w";
//        id << "X_x";

//        QSqlQuery query( QSqlDatabase::database("AI") );
//        query.prepare("insert into advance(key1,id) values(?,?)");
//        query.addBindValue(keys);
//        query.addBindValue(id);
//        qDebug() << QSqlDatabase::database("AI").transaction();
//        bool ret = query.execBatch();
//        if (!ret) {
//            qDebug() << query.lastError();
//            qDebug() << query.lastQuery();
//        }
//        QSqlDatabase::database("AI").commit();
//    }
//    QSqlDatabase::database("AI").close();
//}

//void ActualTimeWidget::setvalue(QString name, double value)
//{
//    if(name == "SBLLQ"){
//        parameter7Wid->setValue(QString::number(value,'f',1));
//    }else if(name == "ZNSJL"){
//        parameter4Wid->setValue(QString::number(value,'f',1));
//    }else if(name == "CY"){
//        parameter3Wid->setValue((QString::number(value,'f',2)));
//    }else if(name == "F"){
//        parameter20Wid->setValue(QString::number(value,'f',1));
//    }else if(name == "QLJ"){
//        parameter17Wid->setValue(QString::number(value,'f',2));
//    }else if(name == "GDYL"){
//        parameter13Wid->setValue(QString::number(value,'f',2));
//    }else if(name == "ZLXS"){
//        parameter12Wid->setValue(QString::number(value,'f',2));
//    }else if(name == "TJF"){
//        parameter2Wid->setValue(QString::number(value,'f',2));
//    }
//}

///* 页面缩放事件 */
//void ActualTimeWidget::resizeEvent(QResizeEvent *event)
//{
//    // 缩放页面时候，背景图随之变化
//    this->findChild<QLabel*>("actual_bgLabel")->resize( this->size() );
//    return;
//}


/**
 * @brief ActualTimeWidget::updateGUI 更新界面结构
 */
void ActualTimeWidget::updateGUI(PARMODELWIDGETFLAG index , float value )
{

    QString text = QString::number(value , 'f' ,2);
    switch ( index )
    {
    case PARMODELWIDGETFLAG::ZLSJLF: //主凝汽节流阀
        this->parameterZlsqjlf->setValue( text );
        break;
    case PARMODELWIDGETFLAG::FLSJLF: //辅凝水节流阀
        this->parameterFlsqjlf->setValue( text);
        break;
    case PARMODELWIDGETFLAG::CYQYLTJF: //除氧器调节阀
        this->parameterCyqyltjf->setValue(text );
        break;
    case PARMODELWIDGETFLAG::LSB_1: //凝水泵1
        this->parameterLsbqlj_1->setValue(text);
        break;
    case PARMODELWIDGETFLAG::LSB_2://凝水泵2
        this->parameterLsbqlj_2->setValue(text);
        break;
    case PARMODELWIDGETFLAG::LSBQLJPQF_1: //凝水泵汽轮机配器伐1
        this->parameterLsbpqf_1->setValue(text);
        break;
    case PARMODELWIDGETFLAG::LSBQLJPQF_2: //凝水泵汽轮机配器伐2
        this->parameterLsbpqf_2->setValue(text);
        break;
    case PARMODELWIDGETFLAG::LSG: //凝水管
        this->parameterLsgzlxs->setValue( text );
        break;
    default:
        break;
    }
}

