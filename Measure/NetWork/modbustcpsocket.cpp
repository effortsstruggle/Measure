#include "modbustcpsocket.h"
#include <QHostAddress>
#include "./../Data/modbusdata.h"
#include <QTimer>

ModbusTcpSocket::ModbusTcpSocket(ModbusData *modbus_data , int socketDesc , QTcpSocket *parent) : QTcpSocket(parent)
{

    this->m_modbusData = modbus_data;

    //设置文件描述符
    this->setSocketDescriptor( socketDesc );
    QObject::connect( this , SIGNAL( error(QAbstractSocket::SocketError) ) , this , SLOT( slot_socketErr(QAbstractSocket::SocketError) ) );

}


ModbusTcpSocket::~ModbusTcpSocket()
{

}

void ModbusTcpSocket::slot_socketErr(QAbstractSocket::SocketError socketError)
{
    ModbusTcpSocket *socket = (ModbusTcpSocket*)sender();
    qDebug()<<"[ModbusTcpSocket::slot_socketErr] socket error :"<< socket->localAddress() <<"  "<< socketError ;
}

/**
 * @brief ModbusTcpSocket::writeModbusData
 */
void ModbusTcpSocket::writeModbusData( QByteArray data_)
{
    this->write( data_ );
}

/**
 * @brief ModbusTcpSocket::slot_readAndParseData 读取并解析数据
 */
void ModbusTcpSocket::slot_readAndParseData()
{
    QByteArray srcData = this->readAll();
    qDebug()<<"[ModbusTcpSocket::slot_readAndParseData] recv data: "<< srcData.toHex();
    QByteArray packet = this->m_modbusData->analysisModbusData( srcData );
    this->writeModbusData(packet);
}
