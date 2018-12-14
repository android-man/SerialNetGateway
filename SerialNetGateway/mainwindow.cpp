#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->serialClearButton, &QPushButton::clicked, this, &MainWindow::OnClickClearSerialData);
    connect(ui->netClearButton, &QPushButton::clicked, this, &MainWindow::OnClickClearTcpData);

    mp_serialAgent = new SerialAgent(this);
    mp_tcpAgent = new TcpAgent(this);
    connect(mp_serialAgent, &SerialAgent::DataReady, this, &MainWindow::OnSerialDataReady);
    connect(mp_tcpAgent, &TcpAgent::DataReady, this, &MainWindow::OnTcpDataReady);
    connect(mp_serialAgent, &SerialAgent::DataReady, mp_tcpAgent, &TcpAgent::SendData);
    connect(mp_tcpAgent, &TcpAgent::DataReady, mp_serialAgent, &SerialAgent::SendData);

    connect(mp_serialAgent, &SerialAgent::AgentReady, this, &MainWindow::OnSerialAgentReady);
    connect(mp_serialAgent, &SerialAgent::AgentDown, this, &MainWindow::OnSerialAgentDown);
    connect(mp_tcpAgent, &TcpAgent::AgentReady, this, &MainWindow::OnTcpAgentReady);
    connect(mp_tcpAgent, &TcpAgent::AgentDown, this, &MainWindow::OnTcpAgentDown);

    QVariantMap serialParam, tcpParam;

    serialParam.insert("SerialName", "COM19");
    serialParam.insert("Parity", QSerialPort::NoParity);
    serialParam.insert("DataBits", QSerialPort::Data8);
    serialParam.insert("StopBits", QSerialPort::OneStop);
    serialParam.insert("BaudRate", QSerialPort::Baud9600);
    serialParam.insert("FlowControl", QSerialPort::NoFlowControl);

    tcpParam.insert("HostName", "114.115.217.241");
//    tcpParam.insert("HostName", "127.0.0.1");
    tcpParam.insert("Port", 8806);

    mp_serialAgent->Start(serialParam);
    mp_tcpAgent->Start(tcpParam);
}

MainWindow::~MainWindow()
{
    mp_serialAgent->Stop();
    mp_tcpAgent->Stop();
    delete ui;
}

void MainWindow::OnSerialDataReady(QByteArray &data)
{
    ui->serialDataEdit->moveCursor(QTextCursor::End);
    ui->serialDataEdit->insertPlainText(QString(data));
}

void MainWindow::OnTcpDataReady(QByteArray &data)
{
    ui->netDataEdit->moveCursor(QTextCursor::End);
    ui->netDataEdit->insertPlainText(QString(data));
}

void MainWindow::OnSerialAgentReady()
{
    ui->serialLabel->setText("串口已打开");
}

void MainWindow::OnSerialAgentDown()
{
    ui->serialLabel->setText("串口已断开");
}

void MainWindow::OnTcpAgentReady()
{
    ui->netLabel->setText("服务器连接已建立");
}

void MainWindow::OnTcpAgentDown()
{
    ui->netLabel->setText("服务器连接已断开");
}

void MainWindow::OnClickClearSerialData()
{
    ui->serialDataEdit->clear();
}

void MainWindow::OnClickClearTcpData()
{
    ui->netDataEdit->clear();
}
