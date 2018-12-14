#ifndef AGENT_H
#define AGENT_H

#include <QObject>
#include <QByteArray>
#include <QSerialPort>
#include <QTcpSocket>
#include <QVariantMap>
#include <QTimer>

class SerialAgent : public QObject
{
    Q_OBJECT

public:
    explicit SerialAgent(QObject *parent = nullptr);
    ~SerialAgent();

    void Start(QVariantMap param);
    void Stop();

private:
    QSerialPort m_serialport;
    QByteArray m_dataBuffer;

signals:
    void DataReady(QByteArray &data);
    void AgentReady();
    void AgentDown();

private slots:
    void OnSerialReadyRead();

public slots:
    void SendData(QByteArray &data);

};


class TcpAgent : public QObject
{
    Q_OBJECT

public:
    explicit TcpAgent(QObject *parent = nullptr);
    ~TcpAgent();

    void Start(QVariantMap param);
    void Stop();

private:
    QTcpSocket  m_tcpSocket;
    QString     m_hostName;
    quint16     m_port;
    bool        m_tcpConnState;
    QTimer      m_reconnectTimer;

signals:
    void DataReady(QByteArray &data);
    void AgentReady();
    void AgentDown();

private slots:
    void OnTcpReadyRead();
    void OnTcpConnected();
    void OnTcpDisconnected();
    void OnTcpError();
    void OnReconnectTimeout();

public slots:
    void SendData(QByteArray &data);
};


#endif // AGENT_H
