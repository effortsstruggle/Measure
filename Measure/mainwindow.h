#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>

//#include <QtCharts/QLineSeries>

class QTabWidget ;
class ModbusTcpServer;
class ModbusData;
class ActualTimeWidget ;
class SoftMeasureWidget;
class QPushButton;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void initGUI();
    void initData();
    void initConnect();
    void initModbus();
    void stackedButClick(int index);
public slots:
//    void on_main_closeBtn_clicked();
protected:
    virtual void resizeEvent(QResizeEvent *event) override;
//    virtual void closeEvent( QCloseEvent *event) override;
private:
    qint8 get_hi_8_bit(qint16 num);
    qint8 get_lo_8_bit(qint16 num);
    float get_float_from_short( short s1 , short s2);
    qint16 get_hi_2_byte( float num );
    qint16 get_lo_2_byte( float num );


signals:
    void connectModbus();
    void disconnectModbus();
    void updatavalue();
private slots:
    void slot_widgetToData();
    void slot_dataToWidget();
//    void slot_controlButClick(QString name);
private:

//    QPushButton *runBut;
//    QPushButton *stopBut;

    QPushButton *m_closeBtn;

    ModbusTcpServer *m_modbusTcpServer ; //modbus服务器

    ModbusData *m_modbusData;//modbus数据

    QTimer* m_widgetToDataTimer;
    QTimer* m_dataToWidgetTimer;

    QStackedWidget *m_bodyStacked;
    QWidget *m_mainWid;
    QPushButton *m_actualTimeButton;
    QPushButton *m_softMeasureButton;
    QTabWidget *m_actualTimeTabWidget;
    ActualTimeWidget *m_actualTimeLeft; //左舷实时监控
    ActualTimeWidget *m_actualTimeRight; //右舷实时监控

    QTabWidget *m_softMeasureTabWidget;
    SoftMeasureWidget *m_softMeasureLeft; //左舷软测量
    SoftMeasureWidget *m_softMeasureRight; //右舷软测量
};
#endif // MAINWINDOW_H
