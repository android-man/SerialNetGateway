#ifndef AGENT_H
#define AGENT_H

#include <QObject>
#include <QByteArray>
#include <QSerialPort>
#include <QTcpSocket>

class SerialAgent : public QObject
{
    Q_OBJECT

public:
    explicit SerialAgent(QObject *parent = nullptr);
    ~SerialAgent();

private:
    QSerialPort m_serialport;
    QByteArray m_dataBuffer;

signals:
    void DataReady(QByteArray &data);
    void AgentReady();

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

private:
    QTcpSocket m_tcpSocket;
    bool m_tcpConnState;

signals:
    void DataReady(QByteArray &data);
    void AgentReady();

private slots:
    void OnTcpReadyRead();
    void OnTcpConnected();

public slots:
    void SendData(QByteArray &data);
};


#endif // AGENT_H
