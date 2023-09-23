#ifndef MODBUSTCPSOCKET_H
#define MODBUSTCPSOCKET_H

#include <QTcpSocket>

class ModbusData;
class QTimer;
class ModbusTcpSocket : public QTcpSocket
{
    Q_OBJECT
public:
    explicit ModbusTcpSocket( ModbusData *m_modbusData , int socketDesc , QTcpSocket *parent = nullptr );
    ~ModbusTcpSocket();

    void writeModbusData( QByteArray data_);

public slots:
    void slot_readAndParseData();

    void slot_socketErr(QAbstractSocket::SocketError socketError);

private:
    ModbusData *m_modbusData;

};

#endif // MODBUSTCPSOCKET_H
