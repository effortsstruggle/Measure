#ifndef MODBUSDATA_H
#define MODBUSDATA_H

#include <QObject>
#include <QTimer>
#include <QMap>
#include "./../Common/common.h"


class ModbusData : public QObject
{
public:

    explicit ModbusData(QObject *parent = nullptr);
    ~ModbusData();

    QByteArray analysisModbusData(QByteArray srcData);

    QByteArray setModbusReadHoldRegisterResponePacket(qint16 workID , qint16 protocolID , qint16 length , qint8 device_addr ,
                                                qint8 function_code , qint8 data_length ,  QVector<qint16> value);

    QByteArray setModbusSignalWriteHoldRegisterResponePacket(qint16 workID , qint16 protocolID , qint16 length , qint8 device_addr ,
                                                             qint8 function_code ,quint16 resigter_addr , qint16 value);

    QByteArray setModbusMultipleWriteHoldRegisterResponePacket(qint16 workID , qint16 protocolID , qint16 length , qint8 device_addr ,
                                                             qint8 function_code ,quint16 resigter_addr , quint16 resigter_num);


    //获取input数据
    qint16 getInputData( GUODIANREGISTERADDR addr_index_ );


    void insertResultData( HUAFENGREGISTERADDR key , qint16 value );
    //获取结果数据
    qint16 getCalcResultData( HUAFENGREGISTERADDR key );

    qint8 get_hi_8_bit( qint16 num );
    qint8 get_lo_8_bit( qint16 num );

    QByteArray big_end_to_small_end(QByteArray data);


private :
    void initAttr();

    void initConnect();

private:
    //国电写入的值
    QMap<GUODIANREGISTERADDR , qint16> m_inputDataMap;
    //华风读取的值
    QMap<HUAFENGREGISTERADDR , qint16> m_resultDataMap;
};

#endif // MODBUSDATA_H
