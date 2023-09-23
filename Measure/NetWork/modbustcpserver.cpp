#include "modbustcpserver.h"
#include "modbustcpsocket.h"
#include "./../Data/modbusdata.h"
#include <QThread>

ModbusTcpServer::ModbusTcpServer(ModbusData * modbus_data , const QString &ip, int port, QTcpServer *parent) : QTcpServer(parent)
{
    this->m_modbusData = modbus_data;

    bool bsucc;
    if (ip.isEmpty() )
    {
        bsucc = this->listen(QHostAddress::AnyIPv4, port);
    }
    else
    {
        bsucc = this->listen( QHostAddress(ip), port );
    }

}



ModbusTcpServer::~ModbusTcpServer()

{

    this->close();

    /*QList<TcpSocket*>::iterator it = m_socketList.begin();

    for (; it != m_socketList.end(); )

    {

    TcpSocket* sock = *it;

    it = m_socketList.erase(it);

    delete sock;

    sock = NULL;

    }

    m_socketList.clear();*/

}



/*

Note: If you want to handle an incoming connection as a new QTcpSocket object in another thread you have to

pass the socketDescriptor to the other thread and create the QTcpSocket object there and use its setSocketDescriptor() method.

*/

void ModbusTcpServer::incomingConnection(qintptr socketDescriptor)
{

    ModbusTcpSocket *socket = new ModbusTcpSocket( this->m_modbusData , socketDescriptor );
    QObject::connect( socket , &ModbusTcpSocket::readyRead , socket , &ModbusTcpSocket::slot_readAndParseData);
    QObject::connect(socket, &ModbusTcpSocket::disconnected , socket , &ModbusTcpSocket::deleteLater );



    QThread *thread = new QThread; // 以socket为父类，当socket释放删除后也会删除线程，或者将线程的quit信号关联槽deleteLater()也可以达到效果
    QObject::connect( socket , &ModbusTcpSocket::disconnected , thread , &QThread::quit);
    //QObject::connect( socket , &ModbusTcpSocket::disconnected , thread , &QThread::terminate);
//    QObject::connect( socket , &ModbusTcpSocket::disconnected , thread , &QThread::wait);
    QObject::connect( socket , &ModbusTcpSocket::disconnected , thread , &QThread::deleteLater);

    socket->moveToThread(thread);
    thread->start();


    emit newConnection(); //文档要求继承本函数需要发射此信号，此处没有与此信号连接的槽
}




