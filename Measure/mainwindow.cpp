#include "mainwindow.h"
#include <QThread>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QStackedWidget>
#include <QFile>
#include <QApplication>
#include <QDebug>
#include <QLabel>
#include <QStyle>
#include <QFont>
#include <QFontDatabase>
#include <QTabWidget>
#include <QSpacerItem>
#include "./Data/modbusdata.h"
#include "./NetWork/modbustcpserver.h"
#include "./Widget/actualtimewidget.h"
#include "./Widget/softmeasurewidget.h"
#include "./NetWork/modbustcpsocket.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    //引入样式文件
    QFile qssFile(":/file/style.qss");
    if(qssFile.open(QFile::ReadOnly)){
        QString styleSheet = QLatin1String(qssFile.readAll());
        qDebug()<<"styleSheet: "<< styleSheet ;
        qApp->setStyleSheet(styleSheet);
        qssFile.close();
        //        qDebug()<< "QSS file load success";
    }else{
        qDebug()<< "ERROR:Qss file load error";
    }


    this->setWindowTitle("软测量程序V1.0");
    this->setMinimumSize(1920,1080);

    //初始化数据层
    this->initData();

    // 初始化页面
    this->initGUI();

    // 初始化Modbus
    this->initModbus();

    //初始化连接
    this->initConnect();

    this->m_widgetToDataTimer->start(2000);
    this->m_dataToWidgetTimer->start(2000);
}

MainWindow::~MainWindow()
{
    if( nullptr != this->m_modbusTcpServer )
    {
        delete this->m_modbusTcpServer;
        this->m_modbusTcpServer = nullptr;
    }
}


/* 页面初始化 */
void MainWindow::initGUI()
{
    //    // 开始按钮
    //    runBut = new QPushButton(this);
    //    runBut->setObjectName("main_runBut");
    //    connect(runBut,&QPushButton::clicked,this,[=](){ this->slot_controlButClick("run"); });
    //    runBut->setGeometry(81+1650,2+70,80,30);
    //    // 暂停按钮
    //    stopBut = new QPushButton(this);
    //    stopBut->setObjectName("main_stopBut");
    //    connect(stopBut,&QPushButton::clicked,this,[=](){this->slot_controlButClick("stop");});
    //    stopBut->setGeometry(176+1650,2+70,80,30);


    // 添加背景图
    QLabel *bgLabel = new QLabel(this);
    bgLabel->setObjectName("main_bgLabel");
    bgLabel ->setPixmap(QPixmap(":/staticFile/images/bg.png"));
    bgLabel->setScaledContents(true);
    // 背景图置于底层
    bgLabel->lower();

    //    //关闭按钮
    //    this->m_closeBtn = new QPushButton(this);
    //    this->m_closeBtn->setObjectName("main_closeBtn");
    //    this->m_closeBtn->setMaximumWidth(80);
    //    this->m_closeBtn->setMaximumHeight(30);
    //    this->m_closeBtn->setGeometry(176+1650,2+10,80,30);
    //    QObject::connect( this->m_closeBtn , &QPushButton::clicked , this , [=](){
    //        qDebug()<<"111111111111111111111";
    //        this->close();
    //    });


    /* 整体布局 */
    this->m_mainWid = new QWidget;
    this->setCentralWidget( this->m_mainWid );

    //整体布局
    QVBoxLayout *mainLayout = new QVBoxLayout( this->m_mainWid );
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setSpacing(0);


    /* 头部名称与按钮组 */
    QWidget *header = new QWidget;
    header->setMaximumHeight(100);
    header->setMinimumHeight(100);
    QHBoxLayout *headLayout = new QHBoxLayout(header);
        headLayout->setContentsMargins(0,0,0,0);
//    headLayout->setContentsMargins(0,0,120,10);
    headLayout->setSpacing(0);

    // 名称
    QWidget *programNameWidget = new QWidget;
    programNameWidget->setObjectName("main_programNameWidget");

    QHBoxLayout *programNameLayout = new QHBoxLayout(programNameWidget);
    programNameLayout->setContentsMargins(20,0,0,25);
//    programNameLayout->setObjectName("main_programNameLayout");

    QLabel *nameLab = new QLabel;
    nameLab->setText("软测量程序");
    nameLab->setMaximumWidth(741);
    nameLab->setObjectName("main_nameLab");
    // 设置名称使用本地字体
    int nfontId = QFontDatabase::addApplicationFont(":/staticFile/dyzgt2.0.ttf");
    if(nfontId>=0)
    {
        QString msyh = QFontDatabase::applicationFontFamilies(nfontId).at(0);
        QFont font(msyh);
        nameLab->setFont(font);
    }
    programNameLayout->addWidget(nameLab);


    /* 页面切换按钮组 */
    QWidget *btnGroupWidget = new QWidget;
    btnGroupWidget->setObjectName("main_btnGroupWidget");

    QHBoxLayout *butGroupLayout = new QHBoxLayout( btnGroupWidget );
    butGroupLayout->setContentsMargins(0,0,0,0);
    butGroupLayout->setSpacing(27);


    // 实时监测按钮
    this->m_actualTimeButton = new QPushButton;
    this->m_actualTimeButton->setMaximumWidth(148);
    this->m_actualTimeButton->setMinimumHeight(36);
    this->m_actualTimeButton->setObjectName("main_actualTimeBut");
    this->m_actualTimeButton->setProperty("click", true); //设置默认选中

    // 软测量检测按钮
    this->m_softMeasureButton = new QPushButton;
    this->m_softMeasureButton->setMaximumWidth(168);
    this->m_softMeasureButton->setMinimumHeight(36);
    this->m_softMeasureButton->setObjectName("main_softMeasureBut");
    QSpacerItem *hSpace = new QSpacerItem(40 , 20 , QSizePolicy::Policy::Expanding);

    butGroupLayout->addWidget(this->m_actualTimeButton);
    butGroupLayout->setStretchFactor(this->m_actualTimeButton,2);
    butGroupLayout->addWidget(this->m_softMeasureButton);
    butGroupLayout->setStretchFactor(this->m_softMeasureButton,2);
    butGroupLayout->addItem(hSpace);


    /* 页面切换按钮组 */
    QWidget *btnCloseWidget = new QWidget;
    btnCloseWidget->setObjectName("main_btnCloseWidget");

    QHBoxLayout *btnCloseLayout = new QHBoxLayout( btnCloseWidget );
    btnCloseLayout->setContentsMargins(245,0,0,60);
    this->m_closeBtn = new QPushButton;
    this->m_closeBtn->setObjectName("main_closeBtn");
    this->m_closeBtn->setMaximumWidth(30);
    this->m_closeBtn->setMinimumHeight(30);
    btnCloseLayout->addWidget(this->m_closeBtn);

    // 头部布局
    headLayout->addWidget(programNameWidget);
    headLayout->setStretchFactor(programNameWidget,4);
    headLayout->addWidget(btnGroupWidget);
    headLayout->setStretchFactor(btnGroupWidget,4);
    headLayout->addWidget(btnCloseWidget);
    headLayout->setStretchFactor(btnCloseWidget,2);



    /* 主体部分 */
    this->m_bodyStacked = new QStackedWidget;

    this->m_actualTimeTabWidget = new QTabWidget;
    this->m_actualTimeTabWidget->setObjectName("actual_tab");

    // 实时监测页面
    this->m_actualTimeLeft = new ActualTimeWidget("左X 2#主凝水节流阀",
                                                  "左X 2#辅凝水节流阀",
                                                  "左X 2#除氧器压力调节阀" ,
                                                  "左X 3#配汽阀" ,
                                                  "左X 4#配汽阀" ,
                                                  "左X 3#凝水泵",
                                                  "左X 4#凝水泵" ,
                                                  "左X 凝水管阻力系数");
    this->m_actualTimeRight = new ActualTimeWidget("右X 1#主凝水节流阀",
                                                   "右X 1#辅凝水节流阀",
                                                   "右X 1#除氧器压力调节阀" ,
                                                   "右X 1#配汽阀" ,
                                                   "右X 2#配汽阀" ,
                                                   "右X 1#凝水泵",
                                                   "右X 2#凝水泵" ,
                                                   "右X 凝水管阻力系数");

    this->m_actualTimeTabWidget->addTab( this->m_actualTimeLeft , "左X" );
    this->m_actualTimeTabWidget->addTab( this->m_actualTimeRight , "右X" );

    // 软测量监控页面
    this->m_softMeasureTabWidget = new QTabWidget ;
    this->m_softMeasureTabWidget->setObjectName("soft_tab");

    this->m_softMeasureLeft = new SoftMeasureWidget("2#主凝水节流调节阀" ,
                                                    "2#辅凝水节流调节阀" ,
                                                    "2#除氧器压力调节阀" ,
                                                    "3#凝水泵" ,
                                                    "4#凝水泵" ,
                                                    "3#凝水泵汽轮机配气阀" ,
                                                    "4#凝水泵汽轮机配气阀",
                                                    "凝水管");

    this->m_softMeasureRight = new SoftMeasureWidget("1#主凝水节流调节阀" ,
                                                     "1#辅凝水节流调节阀" ,
                                                     "1#除氧器压力调节阀" ,
                                                     "1#凝水泵" ,
                                                     "2#凝水泵" ,
                                                     "1#凝水泵汽轮机配气阀" ,
                                                     "2#凝水泵汽轮机配气阀",
                                                     "凝水管");

    this->m_softMeasureTabWidget->addTab( this->m_softMeasureLeft , "左X" );
    this->m_softMeasureTabWidget->addTab( this->m_softMeasureRight , "右X" );

    this->m_bodyStacked->addWidget( this->m_actualTimeTabWidget );
    this->m_bodyStacked->addWidget( this->m_softMeasureTabWidget );

    this->m_bodyStacked->setCurrentIndex(0);


    /* 整体布局 */
    mainLayout->addWidget(header);
    mainLayout->addWidget( this->m_bodyStacked );
    //    mainLayout->addWidget( this->m_closeBtn );

    return ;

}

/**
 * @brief MainWindow::initData 初始化数据层
 */
void MainWindow::initData()
{
    this->m_modbusData = new ModbusData(this);
    this->m_widgetToDataTimer = new QTimer(this);
    this->m_dataToWidgetTimer = new QTimer(this);
}

/**
 * @brief MainWindow::initConnect 初始化信号槽链接
 */
void MainWindow::initConnect()
{
    QObject::connect( this->m_actualTimeButton , &QPushButton::clicked , this , [=](){ this->stackedButClick(0); });
    QObject::connect(this->m_softMeasureButton , &QPushButton::clicked , this , [=](){ this->stackedButClick(1); } );

    //取界面数据放置数据层中
    QObject::connect( this->m_widgetToDataTimer , &QTimer::timeout , this , &MainWindow::slot_widgetToData);
    QObject::connect( this->m_dataToWidgetTimer , &QTimer::timeout , this , &MainWindow::slot_dataToWidget);


    QObject::connect( this->m_closeBtn , &QPushButton::clicked , this , [=](){
        qDebug()<<"111111111111111111111";
        this->close();
    });
}

/* 顶部导航按钮点击事件 */
void MainWindow::stackedButClick(int index){

    // index 0为实时监测按钮被按下，1为软测量检测按钮被按下
    this->m_bodyStacked->setCurrentIndex(index);

    if(index==0){
        this->m_actualTimeButton->setProperty("click", true);
        this->m_softMeasureButton->setProperty("click",false);
    }else {
        this->m_actualTimeButton->setProperty("click", false);
        this->m_softMeasureButton->setProperty("click",true);
    }


    //应用当前最新样式
    this->m_actualTimeButton->style()->polish( this->m_actualTimeButton );
    this->m_softMeasureButton->style()->polish( this->m_softMeasureButton );
    return;
}



/* 页面缩放事件 */
void MainWindow::resizeEvent(QResizeEvent *event)
{
    // 缩放页面时候，背景图随之变化
    this->findChild<QLabel*>("main_bgLabel")->resize( this->size() );
    return;
}

//void MainWindow::closeEvent(QCloseEvent *event)
//{

//}

void MainWindow::slot_widgetToData()
{
    /********************************定时取ModbusData数据,显示在界面上 "实时监控界面" 及 "软测量界面"数据，软测量界面数据需要计算********************************/
    for( int i = 0 ; i < PARMODELWIDGETFLAG::ERROR ; i++ )
    {
        PARMODELWIDGETFLAG index  = (PARMODELWIDGETFLAG)i;

        float zxResultValue = this->m_softMeasureLeft->getResultValue( index );
        float yxResultValue = this->m_softMeasureRight->getResultValue( index );

        switch ( index ) {
        case PARMODELWIDGETFLAG::ZLSJLF: //主凝汽节流阀
        {
            this->m_modbusData->insertResultData( HUAFENGREGISTERADDR::YXZLSJLFREGISTER_1 , this->get_hi_2_byte( yxResultValue ) );
            this->m_modbusData->insertResultData( HUAFENGREGISTERADDR::YXZLSJLFREGISTER_2 , this->get_lo_2_byte( yxResultValue ) );

            this->m_modbusData->insertResultData( HUAFENGREGISTERADDR::ZXZLSJLFREGISTER_1 , this->get_hi_2_byte( zxResultValue ) );
            this->m_modbusData->insertResultData( HUAFENGREGISTERADDR::ZXZLSJLFREGISTER_2 , this->get_lo_2_byte( zxResultValue ) );


            this->m_actualTimeRight->updateGUI(PARMODELWIDGETFLAG::ZLSJLF , yxResultValue );
            this->m_actualTimeLeft->updateGUI(PARMODELWIDGETFLAG::ZLSJLF , zxResultValue );
        }
            break;
        case PARMODELWIDGETFLAG::FLSJLF: //辅凝水节流阀
        {
            this->m_modbusData->insertResultData( HUAFENGREGISTERADDR::YXFLSJLFREGISTER_1 , this->get_hi_2_byte(yxResultValue));
            this->m_modbusData->insertResultData( HUAFENGREGISTERADDR::YXFLSJLFREGISTER_2 , this->get_lo_2_byte(yxResultValue));

            this->m_modbusData->insertResultData( HUAFENGREGISTERADDR::ZXFLSJLFREGISTER_1 , this->get_hi_2_byte(zxResultValue));
            this->m_modbusData->insertResultData( HUAFENGREGISTERADDR::ZXFLSJLFREGISTER_2 , this->get_lo_2_byte(zxResultValue));

            this->m_actualTimeRight->updateGUI(PARMODELWIDGETFLAG::FLSJLF , yxResultValue );
            this->m_actualTimeLeft->updateGUI(PARMODELWIDGETFLAG::FLSJLF , zxResultValue );
        }
            break;
        case PARMODELWIDGETFLAG::CYQYLTJF: //除氧器调节阀
        {
            this->m_modbusData->insertResultData( HUAFENGREGISTERADDR::YXCYQYLTJFREGISTER_1 , this->get_hi_2_byte(yxResultValue));
            this->m_modbusData->insertResultData( HUAFENGREGISTERADDR::YXCYQYLTJFREGISTER_2 , this->get_lo_2_byte(yxResultValue));

            this->m_modbusData->insertResultData( HUAFENGREGISTERADDR::ZXCYQYLTJFREGISTER_1 , this->get_hi_2_byte(zxResultValue));
            this->m_modbusData->insertResultData( HUAFENGREGISTERADDR::ZXCYQYLTJFREGISTER_2 , this->get_lo_2_byte(zxResultValue));

            this->m_actualTimeRight->updateGUI(PARMODELWIDGETFLAG::CYQYLTJF , yxResultValue );
            this->m_actualTimeLeft->updateGUI(PARMODELWIDGETFLAG::CYQYLTJF , zxResultValue );
        }
            break;
        case PARMODELWIDGETFLAG::LSB_1: //凝水泵1
        {
            this->m_modbusData->insertResultData( HUAFENGREGISTERADDR::YXLSB_1REGISTER_1 , this->get_hi_2_byte(yxResultValue));
            this->m_modbusData->insertResultData( HUAFENGREGISTERADDR::YXLSB_1REGISTER_2 , this->get_lo_2_byte(yxResultValue));

            this->m_modbusData->insertResultData( HUAFENGREGISTERADDR::ZXLSB_1REGISTER_1 , this->get_hi_2_byte(zxResultValue));
            this->m_modbusData->insertResultData( HUAFENGREGISTERADDR::ZXLSB_1REGISTER_2 , this->get_lo_2_byte(zxResultValue));

            this->m_actualTimeRight->updateGUI(PARMODELWIDGETFLAG::LSB_1 , yxResultValue );
            this->m_actualTimeLeft->updateGUI(PARMODELWIDGETFLAG::LSB_1 , zxResultValue );
        }
            break;
        case PARMODELWIDGETFLAG::LSB_2://凝水泵2
        {
            this->m_modbusData->insertResultData( HUAFENGREGISTERADDR::YXLSB_2REGISTER_1 , this->get_hi_2_byte(yxResultValue));
            this->m_modbusData->insertResultData( HUAFENGREGISTERADDR::YXLSB_2REGISTER_2 , this->get_lo_2_byte(yxResultValue));

            this->m_modbusData->insertResultData( HUAFENGREGISTERADDR::ZXLSB_2REGISTER_1 , this->get_hi_2_byte(zxResultValue));
            this->m_modbusData->insertResultData( HUAFENGREGISTERADDR::ZXLSB_2REGISTER_2 , this->get_lo_2_byte(zxResultValue));

            this->m_actualTimeRight->updateGUI(PARMODELWIDGETFLAG::LSB_2 , yxResultValue );
            this->m_actualTimeLeft->updateGUI(PARMODELWIDGETFLAG::LSB_2 , zxResultValue );
        }
            break;
        case PARMODELWIDGETFLAG::LSBQLJPQF_1: //凝水泵汽轮机配器伐1
        {
            this->m_modbusData->insertResultData( HUAFENGREGISTERADDR::YXLSBQLJPQF_1REGISTER_1 , this->get_hi_2_byte(yxResultValue));
            this->m_modbusData->insertResultData( HUAFENGREGISTERADDR::YXLSBQLJPQF_1REGISTER_2 , this->get_lo_2_byte(yxResultValue));

            this->m_modbusData->insertResultData( HUAFENGREGISTERADDR::ZXLSBQLJPQF_1REGISTER_1 , this->get_hi_2_byte(zxResultValue));
            this->m_modbusData->insertResultData( HUAFENGREGISTERADDR::ZXLSBQLJPQF_1REGISTER_2 , this->get_lo_2_byte(zxResultValue));

            this->m_actualTimeRight->updateGUI(PARMODELWIDGETFLAG::LSBQLJPQF_1 , yxResultValue );
            this->m_actualTimeLeft->updateGUI(PARMODELWIDGETFLAG::LSBQLJPQF_1 , zxResultValue );
        }
            break;
        case PARMODELWIDGETFLAG::LSBQLJPQF_2: //凝水泵汽轮机配器伐2
        {
            this->m_modbusData->insertResultData( HUAFENGREGISTERADDR::YXLSBQLJPQF_2REGISTER_1 , this->get_hi_2_byte(yxResultValue));
            this->m_modbusData->insertResultData( HUAFENGREGISTERADDR::YXLSBQLJPQF_2REGISTER_2 , this->get_lo_2_byte(yxResultValue));

            this->m_modbusData->insertResultData( HUAFENGREGISTERADDR::ZXLSBQLJPQF_2REGISTER_1 , this->get_hi_2_byte(zxResultValue));
            this->m_modbusData->insertResultData( HUAFENGREGISTERADDR::ZXLSBQLJPQF_2REGISTER_2 , this->get_lo_2_byte(zxResultValue));


            this->m_actualTimeRight->updateGUI(PARMODELWIDGETFLAG::LSBQLJPQF_2 , yxResultValue );
            this->m_actualTimeLeft->updateGUI(PARMODELWIDGETFLAG::LSBQLJPQF_2 , zxResultValue );
        }
            break;
        case PARMODELWIDGETFLAG::LSG: //凝水管
        {
            this->m_modbusData->insertResultData( HUAFENGREGISTERADDR::YXLSGREGISTER_1 , this->get_hi_2_byte(yxResultValue));
            this->m_modbusData->insertResultData( HUAFENGREGISTERADDR::YXLSGREGISTER_2 , this->get_lo_2_byte(yxResultValue));

            this->m_modbusData->insertResultData( HUAFENGREGISTERADDR::YXLSGREGISTER_1 , this->get_hi_2_byte(zxResultValue));
            this->m_modbusData->insertResultData( HUAFENGREGISTERADDR::YXLSGREGISTER_2 , this->get_lo_2_byte(zxResultValue));

            this->m_actualTimeRight->updateGUI(PARMODELWIDGETFLAG::LSG , yxResultValue );
            this->m_actualTimeLeft->updateGUI(PARMODELWIDGETFLAG::LSG , zxResultValue );
        }
            break;
        default:
            break;
        }
    }

    //        qDebug()<<" this->m_modbusData :"<<  this->m_modbusData ;
    //        //测试是否写入成功
    //        for( int i = 0 ; i < PARMODELWIDGETFLAG::ERROR ; i++ )
    //        {

    //            PARMODELWIDGETFLAG index  = (PARMODELWIDGETFLAG)i;

    //            switch ( index ) {
    //            case ZLSJLF:
    //            case FLSJLF:
    //            case CYQYLTJF:
    //            case LSBQLJPQF_1:
    //            case LSBQLJPQF_2:
    //            {
    //                qDebug()<<"index_ :"<< index
    //                       <<" zx_value: "<< this->m_modbusData->getZxResultData(index).valve.realTimeFlow
    //                      <<" yx_value: "<<this->m_modbusData->getYxResultData(index).valve.realTimeFlow;
    //            }
    //                break;
    //            case LSB_1:
    //            case LSB_2:
    //            {

    //                qDebug()<<"index_ :"<< index
    //                       <<" zx_value: "<< this->m_modbusData->getZxResultData(index).pump.realTimeFlow
    //                      <<" yx_value: "<<this->m_modbusData->getYxResultData(index).pump.realTimeFlow;
    //            }
    //                break;
    //            case LSG:
    //            {
    //                qDebug()<<"index_ :"<< index
    //                       <<" zx_value: "<< this->m_modbusData->getZxResultData(index).pipe.realTimeFlow
    //                      <<" yx_value: "<<this->m_modbusData->getYxResultData(index).pipe.realTimeFlow;
    //            }
    //                break;
    //            default:
    //                break;
    //            }

    //        }


}

void MainWindow::slot_dataToWidget()
{
    for( int i = 0 ; i < PARMODELWIDGETFLAG::ERROR ; i++ )
    {
        PARMODELWIDGETFLAG index  = (PARMODELWIDGETFLAG)i;

        float yxData_1;
        float yxData_2;
        float yxData_3;

        float zxData_1;
        float zxData_2;
        float zxData_3;

        switch ( index ) {
        case PARMODELWIDGETFLAG::ZLSJLF: //主凝汽节流阀
        {
            yxData_1 = this->get_float_from_short( this->m_modbusData->getInputData( GUODIANREGISTERADDR::YXZLSJLFKD_1 ) ,  this->m_modbusData->getInputData( GUODIANREGISTERADDR::YXZLSJLFKD_2 ) );
            yxData_2 = this->get_float_from_short( this->m_modbusData->getInputData( GUODIANREGISTERADDR::YXZLSJLFFQYL_1 ) , this->m_modbusData->getInputData( GUODIANREGISTERADDR::YXZLSJLFFQYL_2 ) );
            yxData_3 = this->get_float_from_short( this->m_modbusData->getInputData( GUODIANREGISTERADDR::YXZLSJLFFHYL_1 ) , this->m_modbusData->getInputData( GUODIANREGISTERADDR::YXZLSJLFFHYL_2 ) );


            zxData_1 =  this->get_float_from_short( this->m_modbusData->getInputData( GUODIANREGISTERADDR::ZXZLSJLFKD_1 ) , this->m_modbusData->getInputData( GUODIANREGISTERADDR::ZXZLSJLFKD_2 ) ) ;
            zxData_2 = this->get_float_from_short( this->m_modbusData->getInputData( GUODIANREGISTERADDR::ZXZLSJLFFQYL_1 ) , this->m_modbusData->getInputData( GUODIANREGISTERADDR::ZXZLSJLFFQYL_2 ) );
            zxData_3 =  this->get_float_from_short( this->m_modbusData->getInputData( GUODIANREGISTERADDR::ZXZLSJLFFHYL_1 ) , this->m_modbusData->getInputData( GUODIANREGISTERADDR::ZXZLSJLFFHYL_2 ) );

            this->m_softMeasureRight->updataGUI( index , yxData_1 , yxData_2 , yxData_3 );
            this->m_softMeasureLeft->updataGUI( index , zxData_1 , zxData_2 , zxData_3);
        }
            break;
        case PARMODELWIDGETFLAG::FLSJLF: //辅凝水节流阀
        {
            yxData_1 = this->get_float_from_short( this->m_modbusData->getInputData( GUODIANREGISTERADDR::YXFLSJLFKD_1 ) , this->m_modbusData->getInputData( GUODIANREGISTERADDR::YXFLSJLFKD_2 ) );
            yxData_2 = this->get_float_from_short( this->m_modbusData->getInputData( GUODIANREGISTERADDR::YXFLSJLFFQYL_1 ) , this->m_modbusData->getInputData( GUODIANREGISTERADDR::YXFLSJLFFQYL_2 ) );
            yxData_3 = this->get_float_from_short( this->m_modbusData->getInputData( GUODIANREGISTERADDR::YXFLSJLFFHYL_1 ) , this->m_modbusData->getInputData( GUODIANREGISTERADDR::YXFLSJLFFHYL_2 ));


            zxData_1 = this->get_float_from_short( this->m_modbusData->getInputData( GUODIANREGISTERADDR::ZXFLSJLFKD_1 ) , this->m_modbusData->getInputData( GUODIANREGISTERADDR::ZXFLSJLFKD_2 ) );
            zxData_2 = this->get_float_from_short( this->m_modbusData->getInputData( GUODIANREGISTERADDR::ZXFLSJLFFQYL_1 ) , this->m_modbusData->getInputData( GUODIANREGISTERADDR::ZXFLSJLFFQYL_2 ) );
            zxData_3 = this->get_float_from_short( this->m_modbusData->getInputData( GUODIANREGISTERADDR::ZXFLSJLFFHYL_1 ) , this->m_modbusData->getInputData( GUODIANREGISTERADDR::ZXFLSJLFFHYL_2 ) );

            this->m_softMeasureRight->updataGUI( index , yxData_1 , yxData_2 , yxData_3 );
            this->m_softMeasureLeft->updataGUI( index , zxData_1 , zxData_2 , zxData_3);
        }
            break;
        case PARMODELWIDGETFLAG::CYQYLTJF: //除氧器调节阀
        {
            yxData_1 = this->get_float_from_short( this->m_modbusData->getInputData( GUODIANREGISTERADDR::YXCYQYLTJF_1 ) , this->m_modbusData->getInputData( GUODIANREGISTERADDR::YXCYQYLTJF_2 ) );
            yxData_2 = this->get_float_from_short( this->m_modbusData->getInputData( GUODIANREGISTERADDR::YXCYQYLTJFFQYL_1 ) , this->m_modbusData->getInputData( GUODIANREGISTERADDR::YXCYQYLTJFFQYL_2 ) );
            yxData_3 = this->get_float_from_short( this->m_modbusData->getInputData( GUODIANREGISTERADDR::YXCYQYLTJFFHYL_1 ) , this->m_modbusData->getInputData( GUODIANREGISTERADDR::YXCYQYLTJFFHYL_2 ) );


            zxData_1 = this->get_float_from_short( this->m_modbusData->getInputData( GUODIANREGISTERADDR::ZXCYQYLTJF_1 ) , this->m_modbusData->getInputData( GUODIANREGISTERADDR::ZXCYQYLTJF_2 ));
            zxData_2 = this->get_float_from_short( this->m_modbusData->getInputData( GUODIANREGISTERADDR::ZXCYQYLTJFFQYL_1 ) , this->m_modbusData->getInputData( GUODIANREGISTERADDR::ZXCYQYLTJFFQYL_2 ) );
            zxData_3 = this->get_float_from_short( this->m_modbusData->getInputData( GUODIANREGISTERADDR::ZXCYQYLTJFFHYL_1 ) , this->m_modbusData->getInputData( GUODIANREGISTERADDR::ZXCYQYLTJFFHYL_2 ) );

            this->m_softMeasureRight->updataGUI( index , yxData_1 , yxData_2 , yxData_3 );
            this->m_softMeasureLeft->updataGUI( index , zxData_1 , zxData_2 , zxData_3);
        }
            break;
        case PARMODELWIDGETFLAG::LSB_1: //凝水泵1
        {
            yxData_1 = this->get_float_from_short( this->m_modbusData->getInputData( GUODIANREGISTERADDR::YXLSBZS_1_1 ) , this->m_modbusData->getInputData( GUODIANREGISTERADDR::YXLSBZS_1_2 ) );
            yxData_2 = this->get_float_from_short( this->m_modbusData->getInputData( GUODIANREGISTERADDR::YXLSBRKYL_1_1 ) , this->m_modbusData->getInputData( GUODIANREGISTERADDR::YXLSBRKYL_1_2 ) );
            yxData_3 = this->get_float_from_short( this->m_modbusData->getInputData( GUODIANREGISTERADDR::YXLSBCKYL_1_1 ) , this->m_modbusData->getInputData( GUODIANREGISTERADDR::YXLSBCKYL_1_2 ) );


            zxData_1 = this->get_float_from_short( this->m_modbusData->getInputData( GUODIANREGISTERADDR::ZXLSBZS_3_1 ) , this->m_modbusData->getInputData( GUODIANREGISTERADDR::ZXLSBZS_3_2 ) );
            zxData_2 = this->get_float_from_short( this->m_modbusData->getInputData( GUODIANREGISTERADDR::ZXLSBRKYL_3_1 ) , this->m_modbusData->getInputData( GUODIANREGISTERADDR::ZXLSBRKYL_3_2 ) );
            zxData_3 = this->get_float_from_short( this->m_modbusData->getInputData( GUODIANREGISTERADDR::ZXLSBCKYL_3_1 ) , this->m_modbusData->getInputData( GUODIANREGISTERADDR::ZXLSBCKYL_3_2 ) );

            this->m_softMeasureRight->updataGUI( index , yxData_1 , yxData_2 , yxData_3 );
            this->m_softMeasureLeft->updataGUI( index , zxData_1 , zxData_2 , zxData_3);
        }
            break;
        case PARMODELWIDGETFLAG::LSB_2://凝水泵2
        {
            yxData_1 = this->get_float_from_short( this->m_modbusData->getInputData( GUODIANREGISTERADDR::YXLSBZS_2_1 ) , this->m_modbusData->getInputData( GUODIANREGISTERADDR::YXLSBZS_2_2 ) );
            yxData_2 = this->get_float_from_short( this->m_modbusData->getInputData( GUODIANREGISTERADDR::YXLSBRKYL_2_1 ) , this->m_modbusData->getInputData( GUODIANREGISTERADDR::YXLSBRKYL_2_2 ) );
            yxData_3 = this->get_float_from_short( this->m_modbusData->getInputData( GUODIANREGISTERADDR::YXLSBCKYL_2_1 ) , this->m_modbusData->getInputData( GUODIANREGISTERADDR::YXLSBCKYL_2_2 ) );


            zxData_1 = this->get_float_from_short( this->m_modbusData->getInputData( GUODIANREGISTERADDR::ZXLSBZS_4_1 ) , this->m_modbusData->getInputData( GUODIANREGISTERADDR::ZXLSBZS_4_2 ) );
            zxData_2 = this->get_float_from_short( this->m_modbusData->getInputData( GUODIANREGISTERADDR::ZXLSBRKYL_4_1 ) , this->m_modbusData->getInputData( GUODIANREGISTERADDR::ZXLSBRKYL_4_2 ) );
            zxData_3 = this->get_float_from_short( this->m_modbusData->getInputData( GUODIANREGISTERADDR::ZXLSBCKYL_4_1 ) , this->m_modbusData->getInputData( GUODIANREGISTERADDR::ZXLSBCKYL_4_2 ) );

            this->m_softMeasureRight->updataGUI( index , yxData_1 , yxData_2 , yxData_3 );
            this->m_softMeasureLeft->updataGUI( index , zxData_1 , zxData_2 , zxData_3);
        }
            break;
        case PARMODELWIDGETFLAG::LSBQLJPQF_1: //凝水泵汽轮机配器伐1
        {
            yxData_1 = this->get_float_from_short( this->m_modbusData->getInputData( GUODIANREGISTERADDR::YXLSBQLJPQFKD_1_1 ) , this->m_modbusData->getInputData( GUODIANREGISTERADDR::YXLSBQLJPQFKD_1_2 ) );
            yxData_2 = this->get_float_from_short( this->m_modbusData->getInputData( GUODIANREGISTERADDR::YXLSBQLJPQFFQYL_1_1 ) ,  this->m_modbusData->getInputData( GUODIANREGISTERADDR::YXLSBQLJPQFFQYL_1_2 ));
            yxData_3 = this->get_float_from_short( this->m_modbusData->getInputData( GUODIANREGISTERADDR::YXLSBQLJPQFFHYL_1_1 ) , this->m_modbusData->getInputData( GUODIANREGISTERADDR::YXLSBQLJPQFFHYL_1_2 ));


            zxData_1 = this->get_float_from_short( this->m_modbusData->getInputData( GUODIANREGISTERADDR::ZXLSBQLJPQFKD_3_1 ) , this->m_modbusData->getInputData( GUODIANREGISTERADDR::ZXLSBQLJPQFKD_3_2 ) );
            zxData_2 = this->get_float_from_short( this->m_modbusData->getInputData( GUODIANREGISTERADDR::ZXLSBQLJPQFFQYL_3_1 ) , this->m_modbusData->getInputData( GUODIANREGISTERADDR::ZXLSBQLJPQFFQYL_3_2 ) );
            zxData_3 = this->get_float_from_short( this->m_modbusData->getInputData( GUODIANREGISTERADDR::ZXLSBQLJPQFFHYL_3_1 ) , this->m_modbusData->getInputData( GUODIANREGISTERADDR::ZXLSBQLJPQFFHYL_3_2 ));

            this->m_softMeasureRight->updataGUI( index , yxData_1 , yxData_2 , yxData_3 );
            this->m_softMeasureLeft->updataGUI( index , zxData_1 , zxData_2 , zxData_3);
        }
            break;
        case PARMODELWIDGETFLAG::LSBQLJPQF_2: //凝水泵汽轮机配器伐2
        {
            yxData_1 = this->get_float_from_short( this->m_modbusData->getInputData( GUODIANREGISTERADDR::YXLSBQLJPQFKD_2_1 ) ,  this->m_modbusData->getInputData( GUODIANREGISTERADDR::YXLSBQLJPQFKD_2_2 ) );
            yxData_2 = this->get_float_from_short( this->m_modbusData->getInputData( GUODIANREGISTERADDR::YXLSBQLJPQFFQYL_2_1 ) , this->m_modbusData->getInputData( GUODIANREGISTERADDR::YXLSBQLJPQFFQYL_2_2 ) );
            yxData_3 = this->get_float_from_short( this->m_modbusData->getInputData( GUODIANREGISTERADDR::YXLSBQLJPQFFHYL_2_1 ) , this->m_modbusData->getInputData( GUODIANREGISTERADDR::YXLSBQLJPQFFHYL_2_2 ));


            zxData_1 = this->get_float_from_short( this->m_modbusData->getInputData( GUODIANREGISTERADDR::ZXLSBQLJPQFKD_4_1 ) , this->m_modbusData->getInputData( GUODIANREGISTERADDR::ZXLSBQLJPQFKD_4_2 ) );
            zxData_2 = this->get_float_from_short( this->m_modbusData->getInputData( GUODIANREGISTERADDR::ZXLSBQLJPQFFQYL_4_1 ) , this->m_modbusData->getInputData( GUODIANREGISTERADDR::ZXLSBQLJPQFFQYL_4_2 ) );
            zxData_3 = this->get_float_from_short( this->m_modbusData->getInputData( GUODIANREGISTERADDR::ZXLSBQLJPQFFHYL_4_1 ) , this->m_modbusData->getInputData( GUODIANREGISTERADDR::ZXLSBQLJPQFFHYL_4_2 ) );

            this->m_softMeasureRight->updataGUI( index , yxData_1 , yxData_2 , yxData_3 );
            this->m_softMeasureLeft->updataGUI( index , zxData_1 , zxData_2 , zxData_3);
        }
            break;
        case PARMODELWIDGETFLAG::LSG: //凝水管
        {
            yxData_1 = this->get_float_from_short( this->m_modbusData->getInputData( GUODIANREGISTERADDR::YXLSGSSLL_1 ) , this->m_modbusData->getInputData( GUODIANREGISTERADDR::YXLSGSSLL_2 ));
            yxData_2 = this->get_float_from_short( this->m_modbusData->getInputData( GUODIANREGISTERADDR::YXLSGJKYL_1 ) , this->m_modbusData->getInputData( GUODIANREGISTERADDR::YXLSGJKYL_2 ));
            yxData_3 = this->get_float_from_short( this->m_modbusData->getInputData( GUODIANREGISTERADDR::YXLSGCKYL_1 ) , this->m_modbusData->getInputData( GUODIANREGISTERADDR::YXLSGCKYL_2 ) );


            zxData_1 = this->get_float_from_short( this->m_modbusData->getInputData( GUODIANREGISTERADDR::ZXLSGSSLL_1 ) , this->m_modbusData->getInputData( GUODIANREGISTERADDR::ZXLSGSSLL_2 ));
            zxData_2 = this->get_float_from_short( this->m_modbusData->getInputData( GUODIANREGISTERADDR::ZXLSGJKYL_1 ) , this->m_modbusData->getInputData( GUODIANREGISTERADDR::ZXLSGJKYL_2 ) );
            zxData_3 = this->get_float_from_short( this->m_modbusData->getInputData( GUODIANREGISTERADDR::ZXLSGCKYL_1 ) , this->m_modbusData->getInputData( GUODIANREGISTERADDR::ZXLSGCKYL_2 ));

            this->m_softMeasureRight->updataGUI( index , yxData_1 , yxData_2 , yxData_3 );
            this->m_softMeasureLeft->updataGUI( index , zxData_1 , zxData_2 , zxData_3);
        }
            break;
        default:
            break;
        }

    }


}

///* 开始、停止按钮点击事件 */
//void MainWindow::slot_controlButClick(QString name)
//{

//    if(name=="run"){
//        runBut->setProperty("click", true);
//        stopBut->setProperty("click", false);
//    }else {
//        runBut->setProperty("click", false);
//        stopBut->setProperty("click", true);
//    }
//    //应用当前最新样式
//    runBut->style()->polish(runBut);
//    stopBut->style()->polish(stopBut);
//    return;
//}

qint8 MainWindow::get_hi_8_bit(qint16 num)
{
    return (num>>8)&0xFF; //先取高8位
}

qint8 MainWindow::get_lo_8_bit(qint16 num)
{
    return num&0xFF;
}

qint16 MainWindow::get_hi_2_byte( float num )
{
    unsigned char farray[4];
    memcpy( farray , &num , sizeof(farray) );
    QByteArray byte;
    byte.push_back( farray[1] );
    byte.push_back( farray[0] );

    quint16 conver_num ;
    memcpy(&conver_num , byte.data() , byte.size() );

    return conver_num;
}


qint16 MainWindow::get_lo_2_byte( float num )
{
    unsigned char farray[4];
    memcpy( farray , &num , sizeof(farray) );
    QByteArray byte;
    byte.push_back( farray[3] );
    byte.push_back( farray[2] );

    quint16 conver_num ;
    memcpy(&conver_num , byte.data() , byte.size() );
    return conver_num;
}


float MainWindow::get_float_from_short(short s1, short s2)
{
    QByteArray ab;
    ab.push_back( get_hi_8_bit( s1 ) );
    ab.push_back( get_lo_8_bit( s1 ) );
    ab.push_back( get_hi_8_bit( s2 ) );
    ab.push_back( get_lo_8_bit( s2 ) );

    float f1 ;
    memcpy( &f1 , ab.data() , ab.size() );
    return f1;
}



void MainWindow::initModbus()
{
    //连接通信线程
    this->m_modbusTcpServer = new ModbusTcpServer( this->m_modbusData , "192.102.1.150" , 20502 );
}


//void MainWindow::on_main_closeBtn_clicked()
//{
//    qDebug()<<"111111111111111111111";
//    this->close();
//}



