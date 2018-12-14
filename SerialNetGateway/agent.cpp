#include "agent.h"

SerialAgent::SerialAgent(QObject *parent) :
    QObject(parent),
    m_serialport(this)
{
//    m_serialport.setPortName("COM19");
//    m_serialport.setParity(QSerialPort::NoParity);
//    m_serialport.setDataBits(QSerialPort::Data8);
//    m_serialport.setStopBits(QSerialPort::OneStop);
//    m_serialport.setBaudRate(QSerialPort::Baud9600);
//    m_serialport.setFlowControl(QSerialPort::NoFlowControl);

//    m_serialport.open(QIODevice::ReadWrite);
}

SerialAgent::~SerialAgent()
{

}

void SerialAgent::Start(QVariantMap param)
{
    unsigned char paramFlag = 0;
    QVariantMap::const_iterator it;
    it = param.find("SerialName");
    if(it != param.constEnd()) {
        QString name;
        name = it.value().toString();
        m_serialport.setPortName(name);
        paramFlag |= 0x01;
    }

    it = param.find("Parity");
    if(it != param.constEnd()) {
        m_serialport.setParity(static_cast<QSerialPort::Parity>(it.value().toInt()));
        paramFlag |= 0x02;
    }

    it = param.find("DataBits");
    if(it != param.constEnd()) {
        m_serialport.setDataBits(static_cast<QSerialPort::DataBits>(it.value().toInt()));
        paramFlag |= 0x04;
    }

    it = param.find("StopBits");
    if(it != param.constEnd()) {
        m_serialport.setStopBits(static_cast<QSerialPort::StopBits>(it.value().toInt()));
        paramFlag |= 0x08;
    }

    it = param.find("BaudRate");
    if(it != param.constEnd()) {
        m_serialport.setBaudRate(static_cast<QSerialPort::BaudRate>(it.value().toInt()));
        paramFlag |= 0x10;
    }

    it = param.find("FlowControl");
    if(it != param.constEnd()) {
        m_serialport.setFlowControl(static_cast<QSerialPort::FlowControl>(it.value().toInt()));
        paramFlag |= 0x20;
    }

    if(paramFlag == 0x3F) {
        connect(&m_serialport, &QSerialPort::readyRead, this, &SerialAgent::OnSerialReadyRead);
        if(m_serialport.open(QIODevice::ReadWrite)) {
            emit AgentReady();
        }
    }
}

void SerialAgent::Stop()
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
    connect(&m_reconnectTimer, &QTimer::timeout, this, &TcpAgent::OnReconnectTimeout);
}

TcpAgent::~TcpAgent()
{

}

void TcpAgent::Start(QVariantMap param)
{
    unsigned char paramFlag = 0;
    QVariantMap::const_iterator it;

    it = param.find("HostName");
    if(it != param.constEnd()) {
        m_hostName = it.value().toString();
        paramFlag |= 0x01;
    }

    it = param.find("Port");
    if(it != param.constEnd()) {
        m_port = static_cast<quint16>(it.value().toUInt());
        paramFlag |= 0x02;
    }

    if(paramFlag == 0x03) {
        connect(&m_tcpSocket, &QTcpSocket::connected, this, &TcpAgent::OnTcpConnected);
        connect(&m_tcpSocket, &QTcpSocket::disconnected, this, &TcpAgent::OnTcpDisconnected);

        m_tcpSocket.connectToHost(m_hostName, m_port, QTcpSocket::ReadWrite);
    }
}

void TcpAgent::Stop()
{
    m_tcpSocket.disconnect();
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
    m_reconnectTimer.stop();
    m_tcpConnState = true;
    connect(&m_tcpSocket, &QTcpSocket::readyRead, this, &TcpAgent::OnTcpReadyRead);
    emit AgentReady();
}

void TcpAgent::OnTcpDisconnected()
{
    m_reconnectTimer.start(3000);
    emit AgentDown();
}

void TcpAgent::OnTcpError()
{

}

void TcpAgent::OnReconnectTimeout()
{
    m_tcpSocket.connectToHost(m_hostName, m_port, QTcpSocket::ReadWrite);
}

void TcpAgent::SendData(QByteArray &data)
{
    if(m_tcpConnState) {
        m_tcpSocket.write(data.data(), data.length());
    }
}
