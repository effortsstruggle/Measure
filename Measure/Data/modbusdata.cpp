#include "modbusdata.h"
#include <QDebug>
#include <QDataStream>
#include <iostream>
#include "./../Common/command.h"

ModbusData::ModbusData(QObject *parent)
    : QObject(parent)
{

    this->initAttr();

    this->initConnect();
}

ModbusData::~ModbusData()
{

}




QByteArray ModbusData::analysisModbusData(QByteArray srcData)
{
    if(srcData.size() < 8)
    {
        qDebug()<<"命令有误！！！";
        return QByteArray();
    }

    qint16 work_id = 0x0000; //事务标识
    qint16 protocol_id = 0x0000; //协议ID
    qint16 length = 0x0000; //数据长度
    qint8 device_addr = 0x00; //设备地址
    qint8 function_code = srcData.at(7); //功能码
    quint16 register_addr; // 对应地址需要+1(解析得到的地址需要+1)
    qDebug()<<"ModbusData::analysisModbusData[function_code]: "<<function_code;

    //事务标识
    QByteArray work_id_byte = srcData.mid(0 , 2);
    QByteArray reverse_work_id = this->big_end_to_small_end(work_id_byte);
    memcpy(&work_id , reverse_work_id.data() , reverse_work_id.size() );

    //设备地址
    device_addr = srcData.at(6);


    if( function_code == 0x03 ) //读数据
    {

        quint16 register_num = 0x0000;
        quint8 data_length = 0x00;

        QByteArray register_addr_byte = srcData.mid(8,2); //对应地址需要+1
        QByteArray reverse_register_addr = this->big_end_to_small_end(register_addr_byte);
        memcpy(&register_addr , reverse_register_addr.data() , reverse_register_addr.size() );
        //register_addr += 1;


        QByteArray register_num_byte = srcData.mid(10,2); //对应地址需要+1
        QByteArray reverse_register_num = this->big_end_to_small_end(register_num_byte);
        memcpy(&register_num , reverse_register_num.data() , reverse_register_num.size() );

        //字节数
        data_length = sizeof(register_num) * register_num ;

        //        qDebug()<<"ModbusData::analysisModbusData[0x03] register_addr : "<<srcData.mid(8,2).toHex() <<"   " << register_addr ;
        //        qDebug()<<"ModbusData::analysisModbusData[0x03] register_num : "<< register_num ;

        QVector<qint16> valueVec;
        if(this->m_resultDataMap.find((HUAFENGREGISTERADDR)register_addr) == this->m_resultDataMap.end())
        {
            for( int i = 0 ; i < register_num ; i++ )
            {
                qint16 value = this->m_inputDataMap.value( ( GUODIANREGISTERADDR )(register_addr+i));
                 qDebug()<<"ModbusData::analysisModbusData[0x03] value : "<< value
                                  <<" ModbusData::analysisModbusData[0x03] HUAFENGREGISTERADDR : "<<sizeof(HUAFENGREGISTERADDR) <<"   "<<( HUAFENGREGISTERADDR )(register_addr+i);
                valueVec.push_back(value);
            }
        }
        else
        {
            for( int i = 0 ; i < register_num ; i++ )
            {
                qint16 value = this->m_resultDataMap.value( ( HUAFENGREGISTERADDR )(register_addr+i));
                 qDebug()<<"ModbusData::analysisModbusData[0x03] value : "<< value
                                  <<" ModbusData::analysisModbusData[0x03] HUAFENGREGISTERADDR : "<<sizeof(HUAFENGREGISTERADDR) <<"   "<<( HUAFENGREGISTERADDR )(register_addr+i);
                valueVec.push_back(value);
            }
        }

        //长度
        length = sizeof(device_addr) + sizeof(function_code) + sizeof(data_length) +  data_length;

        QByteArray packet = this->setModbusReadHoldRegisterResponePacket( work_id , protocol_id , length , device_addr , function_code ,  data_length , valueVec);
        //        qDebug()<<"ModbusData::analysisModbusData[0x03] packet : "<<packet.toHex() ;
        //组包
        return packet;

    }
    else if( function_code == 0x06) //取数据
    {
        qint16 data; //待解析的数据

        QByteArray register_addr_byte = srcData.mid(8,2); //对应地址需要+1
        QByteArray reverse_register_addr = this->big_end_to_small_end(register_addr_byte);
        memcpy(&register_addr , reverse_register_addr.data() , reverse_register_addr.size() );
        //register_addr += 1;

        length = sizeof(device_addr) + sizeof(function_code) + sizeof(register_addr)+sizeof(data) ;

        qDebug()<<"ModbusData::analysisModbusData[0x06] register_addr : " <<srcData.mid(8,2).toHex() <<"  "<<register_addr;
        QByteArray byte = srcData.mid(10 , 2) ;
        QByteArray reverse_value_byte = this->big_end_to_small_end(byte);
        memcpy(&data , reverse_value_byte.data() , reverse_value_byte.size() );

        qDebug()<<"ModbusData::analysisModbusData[0x06] data : "<< reverse_value_byte.toHex() << "   " << data  <<"   "<< register_addr;
        //组包
        QByteArray packet = this->setModbusSignalWriteHoldRegisterResponePacket(work_id , protocol_id , length , device_addr , function_code , register_addr, data);
        qDebug()<<" ModbusData::analysisModbusData[0x06] packet : "<<packet.toHex() ;

        //插入数据
        this->m_inputDataMap.insert( (GUODIANREGISTERADDR)register_addr , data );
       // this->m_resultDataMap.insert( (HUAFENGREGISTERADDR)register_addr , data );
        qDebug()<<"[ModbusTcpSocket::analysisModbusData] input data: "<<this->m_inputDataMap;
        return packet;
    }
    else if( function_code == 0x10) //取多个数据
    {
        quint16 register_num = 0x0000;
        quint8 data_length = 0x00;

        QByteArray register_addr_byte = srcData.mid(8,2); //对应地址需要+1
        QByteArray reverse_register_addr = this->big_end_to_small_end(register_addr_byte);
        memcpy(&register_addr , reverse_register_addr.data() , reverse_register_addr.size() );
       // register_addr += 1;

        QByteArray register_num_byte = srcData.mid(10,2);
        QByteArray reverse_register_num = this->big_end_to_small_end(register_num_byte);
        memcpy(&register_num , reverse_register_num.data() , reverse_register_num.size() );

        //字节数量
        data_length = srcData.at(12) ;


        quint16 temp_addr_count = register_addr;
        for( int i = 0 ; i < register_num ; i++ )
        {
            qint16 data; //待解析的数据
            QByteArray byte = srcData.mid( 13 + (i*2) , 2) ;
            //QByteArray reverse_value_byte = this->big_end_to_small_end(byte);
            memcpy(&data , byte.data() , byte.size() );


            //插入数据
            this->m_inputDataMap.insert( (GUODIANREGISTERADDR)temp_addr_count , data );

            //寄存器地址
            temp_addr_count += 1;
        }

        qDebug()<<"[ModbusTcpSocket::analysisModbusData] input data: "<<this->m_inputDataMap;

        //长度

        //length = sizeof(device_addr) + sizeof(function_code) + sizeof(register_addr)  + sizeof( register_num ) + sizeof(data_length) +  data_length;
        length = sizeof(device_addr) + sizeof(function_code) + sizeof(register_addr)  + sizeof( register_num );
        QByteArray packet = this->setModbusMultipleWriteHoldRegisterResponePacket(work_id , protocol_id ,length , device_addr , function_code , register_addr , register_num);
        qDebug()<<" ModbusData::analysisModbusData[0x06] packet : "<<packet.toHex() ;
        return packet;
    }
    else
    {
        qDebug()<<"ModbusData::analysisModbusData[未知功能码，需要拓展！]";
        return  QByteArray();
    }




}



/**
 * @brief ModbusData::setReadHoldRegisterResponePacket “0x03读取保持寄存器" 响应协议
 * @param workID
 * @param start_addr_
 * @param reg_num_
 * @param value_1
 * @param value_2
 * 主站询问报文格式
 * | ------------------------------------------------------------------------------------------------------------------------- |
 *  事务ID  |   协议   |   长度  |单元标识符 |  功能码 | 起始寄存器（高位） | 起始寄存器（低位） | 寄存器数量（高位） |  寄存器数量（低位）
 *  2字节      2字节      2字节     1字节       1字节       1字节              1字节                1字节             1字节
 *| -------------------------------------------------------------------------------------------------------------------------- |
 *  起始寄存器需要+1
 *
 *  从站应答报文格式
 * | ------------------------------------------------------------------------------------------------------------------------------- |
 *  事务ID  |   协议   |   长度  |单元标识符 |  功能码 |  数据长度 |  数据1（高位） | 数据1（低位） | .........|  数据n（高位） | 数据n（低位） |
 *  2字节      2字节      2字节     1字节       1字节     1字节             根据起始寄存器及寄存器数量判断 （不固定）
 * | ------------------------------------------------------------------------------------------------------------------------------- |
 * @return
 */
QByteArray ModbusData::setModbusReadHoldRegisterResponePacket(qint16 workID , qint16 protocolID , qint16 length , qint8 device_addr ,
                                                              qint8 function_code , qint8 data_length , QVector<qint16> valueVec )
{
    PACKET_HEAD packet_head;
    packet_head.work_id_hi = this->get_hi_8_bit(workID);
    packet_head.work_id_lo = this->get_lo_8_bit(workID);
    packet_head.procotol_id = protocolID;
    packet_head.length_hi = this->get_hi_8_bit(length);
    packet_head.length_lo = this->get_lo_8_bit(length);
    packet_head.device_addr = device_addr;

    QByteArray serialize_packet;
    serialize_packet.append( (char*)&packet_head , sizeof (PACKET_HEAD) );
    serialize_packet.append( (char*)&function_code , sizeof(function_code) );
    serialize_packet.append( (char*)&data_length , sizeof(data_length) );
    for( int i = 0 ; i < valueVec.size() ; i++)
    {
        qint16 value = valueVec.at(i);
        serialize_packet.append((char*)&value , sizeof ( value ));
    }
    qDebug()<<"serialize_packet: "<<serialize_packet.toHex();
    return serialize_packet;
}

/**
 * @brief ModbusData::setModbusSignalWriteHoldRegisterResponePacket 写单个保持寄存器的值
 * @param workID
 * @param protocolID
 * @param length
 * @param device_addr
 * @param function_code
 * @param resigter_addr
 * @param value
 * @return
 */
QByteArray ModbusData::setModbusSignalWriteHoldRegisterResponePacket(qint16 workID,
                                                                     qint16 protocolID,
                                                                     qint16 length,
                                                                     qint8 device_addr,
                                                                     qint8 function_code,
                                                                     quint16 resigter_addr,
                                                                     qint16 value)
{
    PACKET_HEAD packet_head;
    packet_head.work_id_hi = this->get_hi_8_bit(workID);
    packet_head.work_id_lo = this->get_lo_8_bit(workID);
    packet_head.procotol_id = protocolID;
    packet_head.length_hi = this->get_hi_8_bit(length);
    packet_head.length_lo = this->get_lo_8_bit(length);
    packet_head.device_addr = device_addr;


    //处理resigter_addr
    qint8 register_addr_hi = this->get_hi_8_bit(resigter_addr);
    qint8 register_addr_lo = this->get_lo_8_bit(resigter_addr);

    qint8 value_hi =  this->get_hi_8_bit(value);
    qint8 value_lo =  this->get_lo_8_bit(value);

    QByteArray serialize_packet;
    serialize_packet.append( (char*)&packet_head , sizeof (PACKET_HEAD) );
    serialize_packet.append( (char*)&function_code , sizeof(function_code) );
    serialize_packet.append( (char*)&register_addr_hi, sizeof(register_addr_hi) );
    serialize_packet.append( (char*)&register_addr_lo, sizeof(register_addr_lo) );
    serialize_packet.append( (char*)&value_hi , sizeof(value_hi) );
    serialize_packet.append( (char*)&value_lo , sizeof(value_lo) );
    return serialize_packet;
}

/**
 * @brief ModbusData::setModbusMultipleWriteHoldRegisterResponePacket 写入多个寄存器响应包
 * @param workID
 * @param protocolID
 * @param length
 * @param device_addr
 * @param function_code
 * @param resigter_addr
 * @param resigter_num
 * @return
 */
QByteArray ModbusData::setModbusMultipleWriteHoldRegisterResponePacket(qint16 workID,
                                                                       qint16 protocolID,
                                                                       qint16 length,
                                                                       qint8 device_addr,
                                                                       qint8 function_code,
                                                                       quint16 resigter_addr,
                                                                       quint16 resigter_num)
{
    PACKET_HEAD packet_head;
    packet_head.work_id_hi = this->get_hi_8_bit(workID);
    packet_head.work_id_lo = this->get_lo_8_bit(workID);
    packet_head.procotol_id = protocolID;
    packet_head.length_hi = this->get_hi_8_bit(length);
    packet_head.length_lo = this->get_lo_8_bit(length);
    packet_head.device_addr = device_addr;


    //处理resigter_addr
    qint8 register_addr_hi = this->get_hi_8_bit(resigter_addr);
    qint8 register_addr_lo = this->get_lo_8_bit(resigter_addr);

    qint8 register_num_hi =  this->get_hi_8_bit(resigter_num);
    qint8 register_num_lo =  this->get_lo_8_bit(resigter_num);

    QByteArray serialize_packet;
    serialize_packet.append( (char*)&packet_head , sizeof (PACKET_HEAD) );
    serialize_packet.append( (char*)&function_code , sizeof(function_code) );
    serialize_packet.append( (char*)&register_addr_hi, sizeof(register_addr_hi) );
    serialize_packet.append( (char*)&register_addr_lo, sizeof(register_addr_lo) );
    serialize_packet.append( (char*)&register_num_hi , sizeof(register_num_hi) );
    serialize_packet.append( (char*)&register_num_lo , sizeof(register_num_lo) );
    return serialize_packet;
}

/**
 * @brief ModbusData::getInputData 获取输入数据
 * @param addr_index_
 * @return
 */
qint16 ModbusData::getInputData(GUODIANREGISTERADDR addr_index_)
{
    qint16 value = this->m_inputDataMap.value(addr_index_);
//    qDebug()<<"--------------"<< addr_index_ << "  " <<value<<"-------------------";
    return value;
}

/**
 * @brief ModbusData::initAttr 初始化属性
 */
void ModbusData::initAttr()
{

}



/**
 * @brief ModbusData::initConnect 初始化连接
 */
void ModbusData::initConnect()
{

}


/**
 * @brief ModbusData::insertResultData 插入结果数据
 * @param key
 * @param value
 */
void ModbusData::insertResultData( HUAFENGREGISTERADDR key , qint16 value )
{
    this->m_resultDataMap.insert( key , value );
}

/**
 * @brief ModbusData::getYxResultData 获取结果的值
 * @param key
 * @return
 */
qint16 ModbusData::getCalcResultData(HUAFENGREGISTERADDR key)
{
    return  this->m_resultDataMap.value(key);
}

qint8 ModbusData::get_hi_8_bit(qint16 num)
{
    return (num>>8)&0xFF; //先取高8位
}

qint8 ModbusData::get_lo_8_bit(qint16 num)
{
    return num&0xFF;
}

/**
 * @brief ModbusData::big_end_to_small_end 大端转换成小端
 * @param data
 * @return
 */
QByteArray ModbusData::big_end_to_small_end(QByteArray data)
{
    std::reverse(data.begin() , data.end() );
    return data;
}










