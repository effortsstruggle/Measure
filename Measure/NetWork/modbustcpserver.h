#ifndef MODBUSTCPSERVER_H
#define MODBUSTCPSERVER_H

#include <QTcpServer>
#include <QQueue>
#include "./../Common/common.h"


class ModbusTcpSocket;
class ModbusData ;
class ModbusTcpServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit ModbusTcpServer(ModbusData * modbus_data  , const QString &ip , int port ,  QTcpServer *parent = nullptr);
    ~ModbusTcpServer();

    virtual void incomingConnection( qintptr socketDescriptor ) override;

    void socketDisconn();

private:
    ModbusData *m_modbusData;
};

#endif // MODBUSTCPSERVER_H
