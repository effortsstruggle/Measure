#include "softmeasurewidget.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QPushButton>
#include <QList>
#include <QDebug>
#include <math.h>
#include "./../Data/modbusdata.h"

SoftMeasureWidget::SoftMeasureWidget( QString zlsjlfName , QString flsjlfName , QString cyqyltfgName ,
                                      QString lsbName_1 , QString lsbName_2 , QString lsbqljpqfName_1 ,
                                      QString lsbqljpqfName_2 , QString lsgName , QWidget *parent) : QWidget(parent)
{
    this->m_zlsjlfName = zlsjlfName;
    this->m_flsjlfName = flsjlfName ;
    this->m_cyqyltjfName = cyqyltfgName;
    this->m_lsbName_1 = lsbName_1 ;
    this->m_lsbName_2 = lsbName_2 ;
    this->m_lsbqljpqfName_1 = lsbqljpqfName_1;
    this->m_lsbqljpqfName_2 = lsbqljpqfName_2 ;
    this->m_lsgName = lsgName ;

    this->initGUI();

    this->initConnect();
}


/* 页面初始化 */
void SoftMeasureWidget::initGUI()
{
    // 页面主布局
    QGridLayout *pageLayout = new QGridLayout(this);
    pageLayout->setContentsMargins(20,4,20,61);
    pageLayout->setSpacing(2);

    /************添加子块1 : 主凝水节流阀*****************/
    QList<QList<QString>> paras_zlsjltjf = {
        {"kd","DCS通讯","开度","45","%"} ,
        {"fqyl","DCS通讯","阀前压力","21","Kpa"} ,
        {"fhyl","DCS通讯 ","阀后压力","15","Kpa"} ,
        /*{"wd","DGS通讯","温度","53","C"},
                    {"lltx","用户输入","流量特性","23","t/h"}*/ };

    QList<QString> names_zlsjltjf = { "icon_01.png" , this->m_zlsjlfName };
    QList<QString> outs_zlsjltjf = {"实际流量","45","t/h"};
    //QWidget *wid1 = this->createModel( paras1 , names1 , outs1 );
    Parmodel *parmodel_zlsjltjf =new Parmodel( PARMODELWIDGETFLAG::ZLSJLF , paras_zlsjltjf , names_zlsjltjf , outs_zlsjltjf , 1 );
    pageLayout->addWidget( parmodel_zlsjltjf , 0 , 0 );

    QObject::connect(parmodel_zlsjltjf->icon1 , &QPushButton::clicked , parmodel_zlsjltjf , &Parmodel::setParameter);
    QObject::connect(parmodel_zlsjltjf , &Parmodel::setvalue , this , &SoftMeasureWidget::setResultValue );
    QObject::connect(parmodel_zlsjltjf , &Parmodel::retLineText , this,[=](){
        //    计算结果
        QString str = parmodel_zlsjltjf->getParasValue("kd_soft_value");
        QString str1 = parmodel_zlsjltjf->getParasValue("fqyl_soft_value");
        QString str2 = parmodel_zlsjltjf->getParasValue("fhyl_soft_value");

        double KV = str.toDouble() * 6.35;
        double DP = str1.toDouble() - str2.toFloat();
        double Qliq = KV*0.1* ( sqrt( DP / 1 ) );
        //             double Qliq = KV;
        parmodel_zlsjltjf->setResultValue("soft_outValue",QString::number(Qliq,'f',1));
        //        emit getSBD("ZNSJL",Qliq);

    });

    this->m_parmodelList.insert( PARMODELWIDGETFLAG::ZLSJLF , parmodel_zlsjltjf);

    /************添加子块2：辅凝水节流阀*****************/
    QList<QList<QString>> paras_flsjltjf = {{"kd","DCS通讯","阀门开度","65","%"},
                                            {"fqyl","DCS通讯","阀前压力","21","KPa"},
                                            {"fhyl","DCS通讯","阀后压力","53","KPa"}};
    QList<QString> names_flsjltjf = {"icon_01.png", this->m_flsjlfName };
    QList<QString> outs_flsjltjf = {"实际流量","32","t/h"};
    //QWidget *wid2 = this->createModel(paras2,names2,outs2);
    Parmodel *parmodel_flsjltjf =new Parmodel(PARMODELWIDGETFLAG::FLSJLF , paras_flsjltjf , names_flsjltjf , outs_flsjltjf , 1);
    pageLayout->addWidget( parmodel_flsjltjf , 0 , 1 );

    QObject::connect( parmodel_flsjltjf->icon1,&QPushButton::clicked, parmodel_flsjltjf,&Parmodel::setParameter);
    QObject::connect( parmodel_flsjltjf , &Parmodel::setvalue,this,&SoftMeasureWidget::setResultValue );
    QObject::connect( parmodel_flsjltjf , &Parmodel::retLineText,this,[=](){
        //    计算结果
        QString str =  parmodel_flsjltjf->getParasValue("kd_soft_value");
        QString str1 =  parmodel_flsjltjf->getParasValue("fqyl_soft_value");
        QString str2 =  parmodel_flsjltjf->getParasValue("fhyl_soft_value");
        double KV = str.toDouble()*2.23;
        double DP = str1.toDouble()-str2.toFloat();
        double Qliq = KV*0.1* (sqrt(DP/1));

        parmodel_flsjltjf->setResultValue("soft_outValue",QString::number(Qliq,'f',1));
        //        emit getSBD("F",Qliq);
    });
    this->m_parmodelList.insert( PARMODELWIDGETFLAG::FLSJLF , parmodel_flsjltjf);

    /************添加子块3：除氧器调节阀*****************/
    QList<QList<QString>> paras_cyqtjf = {{"kd","DCS通讯","开度","45.00","%"},
                                          {"fqyl","DCS通讯","阀前压力","21","kPa"},
                                          {"fhyl","DCS通讯","阀后压力","15","kPa"}};
    QList<QString> names_cyqtjf = {"img_07.png", this->m_cyqyltjfName };
    QList<QString> outs_cyqtjf = {"实际流量","45","t/h"};
    //QWidget *wid3 = this->createModel(paras3,names3,outs3);
    Parmodel *parmodel_cyqtjf =new Parmodel( PARMODELWIDGETFLAG::CYQYLTJF , paras_cyqtjf,names_cyqtjf,outs_cyqtjf,1);
    pageLayout->addWidget( parmodel_cyqtjf , 0 , 2 );

    QObject::connect( parmodel_cyqtjf->icon1,&QPushButton::clicked, parmodel_cyqtjf,&Parmodel::setParameter);
    QObject::connect( parmodel_cyqtjf,&Parmodel::setvalue,this,&SoftMeasureWidget::setResultValue );
    QObject::connect( parmodel_cyqtjf,&Parmodel::retLineText,this,[=](){
        //    计算结果
        QString str = parmodel_cyqtjf->getParasValue("kd_soft_value");
        QString str1 = parmodel_cyqtjf->getParasValue("fqyl_soft_value");
        QString str2 = parmodel_cyqtjf->getParasValue("fhyl_soft_value");
        QString v1 = parmodel_cyqtjf->value1->text();
        double KV = str.toDouble()*26.0;
        double DP = str1.toDouble()-str2.toFloat();
        double XX = DP / str1.toDouble();
        double XTP = 0.9;
        double FR = 1.3 / 1.4;
        double XCHOCK = FR * XTP;
        double QGAS = 3.16 * KV * (1 - XX / (3 * XCHOCK)) * pow((DP * v1.toDouble()), 0.5) / 1000.0;
        parmodel_cyqtjf->setResultValue("soft_outValue",QString::number(QGAS,'f',2));
        //        emit getSBD("CY",QGAS);
    });
    this->m_parmodelList.insert( PARMODELWIDGETFLAG::CYQYLTJF , parmodel_cyqtjf);

    /************添加子块4：水泵流量*******************/
    QList<QList<QString>> paras_sbll = {{"zs","DCS通讯","转速","3000","rpm"},
                                        {"bqyl","DCS通讯","泵前压力","0.211","Mpa"},
                                        {"bhyl","DCS通讯","泵后压力","0.104","Mpa"}};
    QList<QString> names_sbll = {"icon_02.png" , this->m_lsbName_1 };
    QList<QString> outs_sbll = {"实际流量","0","m3/h"};
    //QWidget *wid4 = this->createModel(paras4,names4,outs4);
    Parmodel *parmodel_sbll =new Parmodel( PARMODELWIDGETFLAG::LSB_1 , paras_sbll , names_sbll , outs_sbll , 1 );
    pageLayout->addWidget( parmodel_sbll , 0 , 3 );

    QObject::connect( parmodel_sbll->icon1,&QPushButton::clicked, parmodel_sbll,&Parmodel::setParameter_1);
    QObject::connect( parmodel_sbll,&Parmodel::setvalue,this,&SoftMeasureWidget::setResultValue);
    QObject::connect( parmodel_sbll,&Parmodel::retLineText,this,[=](){

        //    计算结果
        QString str =  parmodel_sbll->getParasValue("zs_soft_value");
        QString str1 =  parmodel_sbll->getParasValue("bqyl_soft_value");
        QString str2 =  parmodel_sbll->getParasValue("bhyl_soft_value");
        double v3 = str2.toDouble() - str1.toDouble();
        double Qf = -58.98561 * v3 * v3  + 29.42041 * v3 + 0.60886;
        parmodel_sbll->setResultValue("soft_outValue",QString::number(Qf,'f',2));
        //        emit getSBD("SBLLQ",Qf);
    });
    this->m_parmodelList.insert( PARMODELWIDGETFLAG::LSB_1 , parmodel_sbll);

    /*****************添加子块5：凝水泵流量***************/
    QList<QList<QString>> paras_nsbll = {{"zs","DCS通讯","转速","3000","rpm"},
                                         {"bqyl","DCS通讯","泵前压力","0.211","Mpa"},
                                         {"bhyl","DCS通讯","泵后压力","0.104","Mpa"}};
    QList<QString> names_nsbll = {"icon_02.png", this->m_lsbName_2 };
    QList<QString> outs_nsbll = {"实际流量","0","m3/h"};
    //QWidget *wid5 = this->createModel(paras5,names5,outs5);
    Parmodel *parmodel_nsbll =new Parmodel( PARMODELWIDGETFLAG::LSB_2 , paras_nsbll , names_nsbll , outs_nsbll , 1 );
    pageLayout->addWidget( parmodel_nsbll , 1 , 0 );

    QObject::connect( parmodel_nsbll->icon1,&QPushButton::clicked, parmodel_nsbll,&Parmodel::setParameter_1);
    QObject::connect( parmodel_nsbll,&Parmodel::setvalue,this,&SoftMeasureWidget::setResultValue);
    QObject::connect( parmodel_nsbll,&Parmodel::retLineText,this,[=](){
        //计算结果
        QString str =  parmodel_nsbll->getParasValue("zs_soft_value");
        QString str1 =  parmodel_nsbll->getParasValue("bqyl_soft_value");
        QString str2 =  parmodel_nsbll->getParasValue("bhyl_soft_value");
        double v3 = str2.toDouble() - str1.toDouble();
        double Qf = -58.98561 * v3 * v3  + 29.42041 * v3 + 0.60886;
        parmodel_nsbll->setResultValue("soft_outValue",QString::number(Qf,'f',2));
        //        emit getSBD("SBLLQ",Qf);
    });
    this->m_parmodelList.insert( PARMODELWIDGETFLAG::LSB_2 , parmodel_nsbll);

    /*************添加子块6：管道阻力**************/
    QList<QList<QString>> paras_gdzl = {{"gwll","DCS通讯","管网流量","0","m3/h"},
                                        {"qdyl","DCS通讯","起点压力","0","KPa"},
                                        {"zdyl","DCS通讯","终点压力","0","KPa"},};
    QList<QString> names_gdzl = {"icon_04.png" , this->m_lsgName };
    QList<QString> outs_gdzl = {"阻力系数","0",""};
    //QWidget *wid6 = this->createModel(paras6,names6,outs6);
    Parmodel *parmodel_gdzl =new Parmodel(PARMODELWIDGETFLAG::LSG , paras_gdzl , names_gdzl , outs_gdzl , 1 );
    pageLayout->addWidget( parmodel_gdzl , 1 , 1 );

    QObject::connect( parmodel_gdzl->icon1 , &QPushButton::clicked ,  parmodel_gdzl , &Parmodel::setParameter_3);
    QObject::connect( parmodel_gdzl,&Parmodel::setvalue,this,&SoftMeasureWidget::setResultValue);
    QObject::connect( parmodel_gdzl,&Parmodel::retLineText,this,[=](){
        QString str = parmodel_gdzl->getParasValue("gwll_soft_value");
        QString str1 = parmodel_gdzl->getParasValue("qdyl_soft_value");
        QString str2 = parmodel_gdzl->getParasValue("zdyl_soft_value");
        double a1 = 0.0123408;
        double A1 = a1;
        double RDP = str1.toDouble()-str2.toFloat();
        double zeta = RDP / (str.toDouble()*str.toDouble()) / A1;
        //         parmodel_gdzl->setParasValue("zlxs_soft_value",QString::number(zeta,'f',2));
        //         parmodel_gdzl->setResultValue("soft_outValue",QString::number(RDP,'f',2));
        parmodel_gdzl->setResultValue("soft_outValue",QString::number(zeta,'f',2));
        //        emit getSBD("GDYL",RDP);
        //        emit getSBD("ZLXS",zeta);
    });
    this->m_parmodelList.insert( PARMODELWIDGETFLAG::LSG , parmodel_gdzl);

    /****************添加子块7：调节阀***************/
    QList<QList<QString>> paras_tjf = {{"kd","DCS通讯","开度","90.8","%"},
                                       {"fqyl","DCS通讯","阀前压力","0.121","Mpa"},
                                       {"fhyl","DCS通讯 ","阀后压力","0.104","Mpa"},};
    QList<QString> names_tjf = {"img_07.png", this->m_lsbqljpqfName_1 };
    QList<QString> outs_tjf = {"实际流量","3.23","m3/h"};
    Parmodel *parmodel_tjf =new Parmodel( PARMODELWIDGETFLAG::LSBQLJPQF_1 , paras_tjf , names_tjf , outs_tjf , 1 );
    pageLayout->addWidget(parmodel_tjf , 1 , 2 );

    QObject::connect(  parmodel_tjf->icon1,&QPushButton::clicked ,  parmodel_tjf , &Parmodel::setParameter);
    QObject::connect(  parmodel_tjf ,&Parmodel::setvalue,this,&SoftMeasureWidget::setResultValue);
    QObject::connect(  parmodel_tjf,&Parmodel::retLineText,this,[=](){
        QString str =  parmodel_tjf->getParasValue("kd_soft_value");
        QString str1 =  parmodel_tjf->getParasValue("fqyl_soft_value");
        QString str2 =  parmodel_tjf->getParasValue("fhyl_soft_value");
        double KV = (str.toDouble() / 100) * 2.6944 + 0.6612 + 0.018;
        double DP = (str1.toDouble()-str2.toFloat()) * 1000;
        double Qliq = KV*0.1* (sqrt(DP/1));
        parmodel_tjf->setResultValue("soft_outValue",QString::number(Qliq,'f',2));
        //        emit getSBD("TJF",Qliq);
    });
    this->m_parmodelList.insert( PARMODELWIDGETFLAG::LSBQLJPQF_1 , parmodel_tjf);



    /****************添加子块8：汽轮机配汽阀***************/
    QList<QList<QString>> paras_qljpqf = {{"kd","DCS通讯","开度","90.8","%"},
                                          {"fqyl","DCS通讯","阀前压力","0.121","Mpa"},
                                          {"fhyl","DCS通讯 ","阀后压力","0.104","Mpa"},};
    QList<QString> names_qljpqf = {"img_07.png",this->m_lsbqljpqfName_2};
    QList<QString> outs_qljpqf = {"实际流量","3.23","m3/h"};
    Parmodel *parmodel_qljpqf =new Parmodel(PARMODELWIDGETFLAG::LSBQLJPQF_2 , paras_qljpqf,names_qljpqf,outs_qljpqf,1);
    pageLayout->addWidget(parmodel_qljpqf , 1 , 3 );

    QObject::connect(  parmodel_qljpqf->icon1,&QPushButton::clicked ,  parmodel_qljpqf , &Parmodel::setParameter);
    QObject::connect(  parmodel_qljpqf ,&Parmodel::setvalue,this,&SoftMeasureWidget::setResultValue);
    QObject::connect(  parmodel_qljpqf,&Parmodel::retLineText,this,[=](){
        QString str =  parmodel_qljpqf->getParasValue("kd_soft_value");
        QString str1 =  parmodel_qljpqf->getParasValue("fqyl_soft_value");
        QString str2 =  parmodel_qljpqf->getParasValue("fhyl_soft_value");
        double KV = (str.toDouble() / 100) * 2.6944 + 0.6612 + 0.018;
        double DP = ( str1.toDouble() - str2.toFloat() ) * 1000;
        double Qliq = KV*0.1* (sqrt(DP/1));
        parmodel_qljpqf->setResultValue("soft_outValue",QString::number(Qliq,'f',2));
        //        emit getSBD("qljpqf",Qliq);
    });

    this->m_parmodelList.insert( PARMODELWIDGETFLAG::LSBQLJPQF_2 , parmodel_qljpqf);
}

/**
 * @brief SoftMeasure::initConnect 初始化连接
 */
void SoftMeasureWidget::initConnect()
{
}

void SoftMeasureWidget::setResultValue(PARMODELWIDGETFLAG index_, QString v1, QString v2, QString v3)
{
    QHash<PARMODELWIDGETFLAG , Parmodel *>::Iterator it_begin_ = this->m_parmodelList.find( index_ ) ;
    if( it_begin_ == this->m_parmodelList.end() )
        return ;

    Parmodel * parmodel = it_begin_.value();
    if( nullptr == parmodel )
        return ;


    switch( index_ )
    {
    case PARMODELWIDGETFLAG::ZLSJLF : //设置“主凝水节流阀”计算结果
    {
        //计算结果
        QString str  = parmodel->getParasValue("kd_soft_value");
        QString str1 = parmodel->getParasValue("fqyl_soft_value");
        QString str2 = parmodel->getParasValue("fhyl_soft_value");

        double KV = str.toDouble()*6.35;
        double DP = str1.toDouble()-str2.toFloat();
        double Qliq = KV*0.1* (sqrt(DP/1));

        parmodel->setResultValue("soft_outValue",QString::number(Qliq));
    }
        break;
    case PARMODELWIDGETFLAG::FLSJLF : //设置“辅凝水节流阀”计算结果
    {
        //    计算结果
        QString str =  parmodel->getParasValue("kd_soft_value");
        QString str1 =  parmodel->getParasValue("fqyl_soft_value");
        QString str2 = parmodel->getParasValue("fhyl_soft_value");
        double KV = str.toDouble()*2.23;
        double DP = str1.toDouble()-str2.toFloat();
        double Qliq = KV*0.1* (sqrt(DP/1));

        parmodel->setResultValue("soft_outValue",QString::number(Qliq));
    }
        break;
    case PARMODELWIDGETFLAG::CYQYLTJF : // 设置“除氧器调节阀”计算结果
    {
        //    计算结果
        QString str = parmodel->getParasValue("kd_soft_value");
        QString str1 = parmodel->getParasValue("fqyl_soft_value");
        QString str2 = parmodel->getParasValue("fhyl_soft_value");
        double KV = str.toDouble()*26.0;
        double DP = str1.toDouble()-str2.toFloat();
        double XX = DP / str1.toDouble();
        double XTP = 0.9;
        double FR = 1.3 / 1.4;
        double XCHOCK = FR * XTP;
        double QGAS = 3.16 * KV * (1 - XX / (3 * XCHOCK)) * pow((DP * v1.toDouble()), 0.5) / 1000.0;

        parmodel->setResultValue("soft_outValue",QString::number(QGAS));
    }
        break;
    case PARMODELWIDGETFLAG::LSB_1: // 设置“水泵流量”计算结果
    {
        //计算结果
        QString str = parmodel->getParasValue("zs_soft_value");
        QString str1 = parmodel->getParasValue("bqyl_soft_value");
        QString str2 = parmodel->getParasValue("bhyl_soft_value");
        double Qf = str.toDouble() * v3.toDouble() * v3.toDouble() + str1.toDouble() * v3.toDouble() + str2.toDouble();
        parmodel->setResultValue("soft_outValue",QString::number(Qf));
    }
        break;
    case PARMODELWIDGETFLAG::LSB_2: // 设置“凝水泵流量”计算结果
    {
        //计算结果
        QString str = parmodel->getParasValue("zs_soft_value");
        QString str1 = parmodel->getParasValue("bqyl_soft_value");
        QString str2 = parmodel->getParasValue("bhyl_soft_value");
        double Qf = str.toDouble() * v3.toDouble() * v3.toDouble() + str1.toDouble() * v3.toDouble() + str2.toDouble();
        parmodel->setResultValue("soft_outValue",QString::number(Qf));
    }
        break;
    case PARMODELWIDGETFLAG::LSBQLJPQF_1: //设置“调节阀”计算结果
    {
        //计算结果
        QString str = parmodel->getParasValue("kd_soft_value");
        QString str1 = parmodel->getParasValue("fqyl_soft_value");
        QString str2 = parmodel->getParasValue("fhyl_soft_values");
        double KV = (str.toDouble() / 100) * 2.6944 + 0.6612 + 0.018;
        double DP = (str1.toDouble()-str2.toFloat()) * 1000;
        double Qliq = KV*0.1* (sqrt(DP/1));
        parmodel->setResultValue("soft_outValue",QString::number(Qliq));
    }
        break;
    case PARMODELWIDGETFLAG::LSBQLJPQF_2: //设置“汽轮机配汽阀”计算结果
    {
        //计算结果
        QString str = parmodel->getParasValue("kd_soft_value");
        QString str1 = parmodel->getParasValue("fqyl_soft_value");
        QString str2 = parmodel->getParasValue("fhyl_soft_values");
        double KV = (str.toDouble() / 100) * 2.6944 + 0.6612 + 0.018;
        double DP = (str1.toDouble()-str2.toFloat()) * 1000;
        double Qliq = KV*0.1* (sqrt(DP/1));
        parmodel->setResultValue("soft_outValue",QString::number(Qliq));
    }
        break;
    case PARMODELWIDGETFLAG::LSG: //设置“管道阻力”计算结果
    {
        QString str = parmodel->getParasValue("gwll_soft_value");
        QString str1 = parmodel->getParasValue("qdyl_soft_value");
        QString str2 = parmodel->getParasValue("zdyl_soft_value");
        double a1 = 0.000123408;
        double A1 = a1;
        double RDP = str1.toDouble()-str2.toFloat();
        double zeta = RDP / (str.toDouble()*str.toDouble()) / A1;
        //     parmodel_gdzl->setResultValue("soft_outValue_1",QString::number(zeta,'f',2));
        //     parmodel_gdzl->setResultValue("soft_outValue",QString::number(RDP,'f',2));
        parmodel->setResultValue("soft_outValue",QString::number(zeta,'f',2));
    }
        break;
    default:

        break;
    }
}



/**
 * @brief SoftMeasureWidget::setParasValue_zlsjltjf 设置 "主凝水节流阀" 参数值
 */
void SoftMeasureWidget::setParasValue_zlsjltjf( QString title , QString value)
{
    QHash<PARMODELWIDGETFLAG , Parmodel *>::Iterator it_begin_ = this->m_parmodelList.find( PARMODELWIDGETFLAG::ZLSJLF ) ;
    if( it_begin_ == this->m_parmodelList.end() )
        return;

    Parmodel * parmodel_zlsjltjf = it_begin_.value();
    if( nullptr == parmodel_zlsjltjf )
        return;

    parmodel_zlsjltjf->setParasValue( title , value );
}



/**
 * @brief SoftMeasureWidget::getResultValue_gdzl 获取“管道阻力”计算结果
 * @return
 */
float SoftMeasureWidget::getResultValue( PARMODELWIDGETFLAG index_)
{
    QHash<PARMODELWIDGETFLAG , Parmodel *>::Iterator it_begin_ = this->m_parmodelList.find( index_ ) ;
    if( it_begin_ == this->m_parmodelList.end() )
        return 0.0;

    Parmodel * parmodel = it_begin_.value();
    if( nullptr == parmodel )
        return 0.0;

    return parmodel->getResultValue("soft_outValue");
}

/**
 * @brief SoftMeasureWidget::updataGUI 获取ModebusData数据刷新界面
 */
void SoftMeasureWidget::updataGUI( PARMODELWIDGETFLAG index , double value_1 , double value_2 , double value_3 )
{
    Parmodel *pParmodel = this->m_parmodelList.value( index );
    if( pParmodel == nullptr )
        return ;
    switch( index )
    {
    case PARMODELWIDGETFLAG::ZLSJLF : //设置“主凝水节流阀”计算结果
    {
        //计算结果
        pParmodel->setParasValue("kd_soft_value" , QString::number(value_1,'f',2));
        pParmodel->setParasValue("fqyl_soft_value" , QString::number(value_2,'f',3));
        pParmodel->setParasValue("fhyl_soft_value" , QString::number(value_3,'f',3));

        double KV = value_1*6.35;
        double DP = value_2 - value_3;
        double Qliq = KV*0.1* (sqrt(DP/1));


        pParmodel->setResultValue("soft_outValue",QString::number(Qliq,'f',2));
    }
        break;
    case PARMODELWIDGETFLAG::FLSJLF : //设置“辅凝水节流阀”计算结果
    {
        //计算结果
        pParmodel->setParasValue("kd_soft_value" , QString::number(value_1,'f',2));
        pParmodel->setParasValue("fqyl_soft_value" , QString::number(value_2,'f',3));
        pParmodel->setParasValue("fhyl_soft_value" , QString::number(value_3,'f',3));

        double KV = value_1*2.23;
        double DP = value_2-value_3;
        double Qliq = KV*0.1* (sqrt(DP/1));


        pParmodel->setResultValue("soft_outValue",QString::number(Qliq,'f',2));
    }
        break;
    case PARMODELWIDGETFLAG::CYQYLTJF : // 设置“除氧器调节阀”计算结果
    {
        //计算结果
        pParmodel->setParasValue("kd_soft_value" , QString::number(value_1,'f',2));
        pParmodel->setParasValue("fqyl_soft_value" , QString::number(value_2,'f',3));
        pParmodel->setParasValue("fhyl_soft_value" , QString::number(value_3,'f',3));

        QString v1 = pParmodel->value1->text();
        double KV = value_1 * 26.0;
        double DP = value_2 - value_3;
        double XX = DP / value_2 ;
        double XTP = 0.9;
        double FR = 1.3 / 1.4;
        double XCHOCK = FR * XTP;
        double QGAS = 3.16 * KV * (1 - XX / (3 * XCHOCK)) * pow((DP * v1.toDouble()), 0.5) / 1000.0;


        pParmodel->setResultValue("soft_outValue",QString::number(QGAS,'f',2));
    }
        break;
    case PARMODELWIDGETFLAG::LSB_1: // 设置“水泵流量”计算结果
    {
        //计算结果
        pParmodel->setParasValue("zs_soft_value" , QString::number(value_1,'f',0));
        pParmodel->setParasValue("bqyl_soft_value" , QString::number(value_2,'f',3));
        pParmodel->setParasValue("bhyl_soft_value" , QString::number(value_3,'f',3));

        double v3 = value_3 - value_2;
        double Qf = value_1 * v3  * v3 + value_2 * v3 + value_3;


        pParmodel->setResultValue("soft_outValue",QString::number(Qf,'f',2));
    }
        break;
    case PARMODELWIDGETFLAG::LSB_2: // 设置“凝水泵流量”计算结果
    {
        //计算结果
        pParmodel->setParasValue("zs_soft_value" , QString::number(value_1,'f',0));
        pParmodel->setParasValue("bqyl_soft_value" , QString::number(value_2,'f',3));
        pParmodel->setParasValue("bhyl_soft_value" , QString::number(value_3,'f',3));

        double v3 = value_3 - value_2;
        double Qf = value_1 * v3  * v3 + value_2 * v3 + value_3;


        pParmodel->setResultValue("soft_outValue",QString::number(Qf,'f',2));
    }
        break;
    case PARMODELWIDGETFLAG::LSBQLJPQF_1: //设置“调节阀”计算结果
    {
        //计算结果
        pParmodel->setParasValue("kd_soft_value" , QString::number(value_1,'f',2));
        pParmodel->setParasValue("fqyl_soft_value" , QString::number(value_2,'f',3));
        pParmodel->setParasValue("fhyl_soft_value" , QString::number(value_3,'f',3));

        double KV = (value_1 / 100) * 2.6944 + 0.6612 + 0.018;
        double DP = (value_2 - value_3 ) * 1000;
        double Qliq = KV*0.1* (sqrt(DP/1));


        pParmodel->setResultValue("soft_outValue",QString::number(Qliq,'f',2));
    }
        break;
    case PARMODELWIDGETFLAG::LSBQLJPQF_2: //设置“汽轮机配汽阀”计算结果
    {
        //计算结果
        pParmodel->setParasValue("kd_soft_value" , QString::number(value_1,'f',2));
        pParmodel->setParasValue("fqyl_soft_value" , QString::number(value_2,'f',3));
        pParmodel->setParasValue("fhyl_soft_value" , QString::number(value_3,'f',3));

        double KV = (value_1 / 100) * 2.6944 + 0.6612 + 0.018;
        double DP = (value_2 - value_3 ) * 1000;
        double Qliq = KV*0.1* (sqrt(DP/1));
        pParmodel->setResultValue("soft_outValue",QString::number(Qliq,'f',2));
    }
        break;
    case PARMODELWIDGETFLAG::LSG: //设置“管道阻力”计算结果
    {
        pParmodel->setParasValue("gwll_soft_value" , QString::number(value_1,'f',3));
        pParmodel->setParasValue("qdyl_soft_value" , QString::number(value_2,'f',3));
        pParmodel->setParasValue("zdyl_soft_value" , QString::number(value_3,'f',3));

        double a1 = 0.000123408;
        double A1 = a1;
        double RDP = value_2 - value_3;
        double zeta = RDP / ( value_1 * value_1 ) / A1;
        //     parmodel_gdzl->setResultValue("soft_outValue_1",QString::number(zeta,'f',2));
        //     parmodel_gdzl->setResultValue("soft_outValue",QString::number(RDP,'f',2));


        pParmodel->setResultValue("soft_outValue",QString::number(zeta,'f',2));
    }
        break;
    default:

        break;
    }


}


/* 生成子模块 */
// 参数1 paras：表示模块的所有参数部分，是一个二维数组，其中每一个子项都是页面中一个参数块，子项中依次是：输入类型，参数名，数值，单位。例如{"DGS通讯","开度","45","%"}
//参数2 names:表示模块的名称部分，list，其中第一个项是名称块icon名称（所有icon默认路径在":/staticFile/images/"下），第二项是模块名称
//参数3 outs：表示模块的输出部分，list，其中第一项是输出的名称，第二项是输出的数值，第三项是输出的单位
//QWidget* SoftMeasure::createModel(QList<QList<QString>> paras,QList<QString> names,QList<QString> outs){

//    /* 子模块 */
//    QWidget *modelWid = new QWidget;
//    modelWid->setObjectName("soft_modelWid");
//    QGridLayout *modelLayout = new QGridLayout(modelWid);
//    modelLayout->setContentsMargins(10,10,10,10);
//    modelLayout->setSpacing(10);


//    for(int i=0;i<paras.length();i++){
//        // 输入类型
//        QLabel *type1 = new QLabel;
//        type1->setMaximumHeight(34);
//        type1->setText(paras[i][0]);
//        type1->setAlignment(Qt::AlignCenter);
//        type1->setObjectName("soft_type");

//        // 参数块
//        QWidget *paraValue = new QWidget;
//        paraValue->setMaximumHeight(34);
//        paraValue->setObjectName("soft_paraValue");
//        QHBoxLayout *paraValueLayout = new QHBoxLayout(paraValue);
//        paraValueLayout->setContentsMargins(4,4,4,4);
//        paraValueLayout->setSpacing(0);
//        // 参数名
//        QLabel *parameter1 = new QLabel;
//        parameter1->setObjectName("soft_parameter");
//        parameter1->setText(paras[i][1]);
//        parameter1->setAlignment(Qt::AlignLeft);
//        parameter1->setAlignment(Qt::AlignVCenter);
//        // 参数值
//        QLabel *value1 = new QLabel;
//        value1->setText(paras[i][2]);
//        value1->setObjectName("soft_value");
//        value1->setAlignment(Qt::AlignRight);
//        // 单位
//        QLabel *unit = new QLabel;
//        unit->setText(paras[i][3]);
//        unit->setObjectName("soft_unit");
//        unit->setAlignment(Qt::AlignLeft);
//        unit->setAlignment(Qt::AlignBottom);
//        // 布局
//        paraValueLayout->addWidget(parameter1);
//        paraValueLayout->setStretchFactor(parameter1,2);
//        paraValueLayout->addWidget(value1);
//        paraValueLayout->setStretchFactor(value1,1);
//        paraValueLayout->addWidget(unit);
//        paraValueLayout->setStretchFactor(unit,1);

//        modelLayout->addWidget(type1,i,0,1,1);
//        modelLayout->addWidget(paraValue,i,1,1,2);
//    }

//    // 模块名称部分
//    QWidget *modelName = new QWidget;
//    modelName->setObjectName("soft_modelName");
//    modelName->setMaximumWidth(100);
//    QVBoxLayout *modelNameLayout = new QVBoxLayout(modelName);
//    modelNameLayout->setContentsMargins(10,0,10,0);
//    modelNameLayout->setSpacing(22);
//    modelNameLayout->setAlignment(Qt::AlignCenter);

//    QHBoxLayout *butLayout = new QHBoxLayout;
//    butLayout->setContentsMargins(0,0,0,0);
//    butLayout->setSpacing(0);
//    QPushButton *icon1 = new QPushButton;
//    icon1->setObjectName("soft_icon");
//    icon1->setIcon(QIcon(":/staticFile/images/"+names[0]));
//    icon1->setIconSize(QSize(47,35));
//    icon1->setMaximumSize(47,35);
//    icon1->setMinimumSize(47,35);
//    connect(icon1,&QPushButton::clicked,this,&SoftMeasure::setParameter);
//    butLayout->addWidget(icon1);

//    QLabel *name1 = new QLabel;
//    name1->setAlignment(Qt::AlignCenter);
//    name1->setMinimumHeight(44);
//    name1->setObjectName("soft_name1");
//    name1->setText(names[1]);
//    // 自动换行
//    name1->setWordWrap(true);

//    modelNameLayout->addLayout(butLayout);
//    modelNameLayout->addWidget(name1);


//    // 计算结果部分
//    QWidget *modelOut = new QWidget;
//    modelOut->setObjectName("soft_modelOut");
//    modelOut->setMaximumSize(110,68);
//    QVBoxLayout *modelOutLayout = new QVBoxLayout(modelOut);
//    modelOutLayout->setContentsMargins(0,0,0,0);
//    modelOutLayout->setSpacing(0);
//    QLabel *outText = new QLabel;
//    outText->setObjectName("soft_outText");
//    outText->setAlignment(Qt::AlignCenter);
//    outText->setText(outs[0]);
//    // 输出值与单位部分
//    QHBoxLayout *valueLayout = new QHBoxLayout;
//    valueLayout->setContentsMargins(0,0,0,0);
//    valueLayout->setSpacing(0);
//    QLabel *outValue = new QLabel;
//    outValue->setObjectName("soft_outValue");
//    outValue->setText(outs[1]);
//    outValue->setAlignment(Qt::AlignRight);
//    QLabel *outUnit = new QLabel;
//    outUnit->setObjectName("soft_outUnit");
//    outUnit->setText(outs[2]);
//    outUnit->setAlignment(Qt::AlignLeft);
//    outUnit->setAlignment(Qt::AlignBottom);
//    valueLayout->addWidget(outValue);
//    valueLayout->addWidget(outUnit);



//    modelOutLayout->addWidget(outText);
//    modelOutLayout->setStretchFactor(outText,2);
//    modelOutLayout->addLayout(valueLayout);
//    modelOutLayout->setStretchFactor(valueLayout,1);


//    modelLayout->addWidget(modelName,0,3,paras.length(),1);

//    modelLayout->addWidget(modelOut,0,4,paras.length(),1);



//    return modelWid;



//}

