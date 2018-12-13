#include "agent.h"

SerialAgent::SerialAgent(QObject *parent) :
    QObject(parent),
    m_serialport(this)
{
    m_serialport.setPortName("COM19");
    m_serialport.setParity(QSerialPort::NoParity);
    m_serialport.setDataBits(QSerialPort::Data8);
    m_serialport.setStopBits(QSerialPort::OneStop);
    m_serialport.setBaudRate(QSerialPort::Baud9600);
    m_serialport.setFlowControl(QSerialPort::NoFlowControl);

    connect(&m_serialport, &QSerialPort::readyRead, this, &SerialAgent::OnSerialReadyRead);
    m_serialport.open(QIODevice::ReadWrite);
}

SerialAgent::~SerialAgent()
{
    m_serialport.close();
}

void SerialAgent::OnSerialReadyRead()
{
    while(m_serialport.bytesAvailable() > 0) {
        QByteArray data;
        data = m_serialport.readAll();
        foreach (char c, data) {
            m_dataBuffer.append(c);
            if(c == '\n') {
                emit DataReady(m_dataBuffer);
                m_dataBuffer.clear();
            }
        }
    }
}

void SerialAgent::SendData(QByteArray &data)
{
    m_serialport.write(data.data(), data.length());
}

TcpAgent::TcpAgent(QObject *parent) :
    QObject(parent),
    m_tcpSocket(this),
    m_tcpConnState(false)
{
    m_tcpSocket.connectToHost("114.115.217.241", 8806, QTcpSocket::ReadWrite);
//    m_tcpSocket.connectToHost("127.0.0.1", 8806, QTcpSocket::ReadWrite);
    connect(&m_tcpSocket, &QTcpSocket::connected, this, &TcpAgent::OnTcpConnected);
}

TcpAgent::~TcpAgent()
{

}

void TcpAgent::OnTcpReadyRead()
{
    while(m_tcpSocket.bytesAvailable() > 0) {
        QByteArray data;
        data = m_tcpSocket.readAll();
        emit DataReady(data);
    }
}

void TcpAgent::OnTcpConnected()
{
    m_tcpConnState = true;
    connect(&m_tcpSocket, &QTcpSocket::readyRead, this, &TcpAgent::OnTcpReadyRead);
}

void TcpAgent::SendData(QByteArray &data)
{
    if(m_tcpConnState) {
        m_tcpSocket.write(data.data(), data.length());
    }
}
