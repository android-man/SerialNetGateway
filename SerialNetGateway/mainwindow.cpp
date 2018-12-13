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
}

MainWindow::~MainWindow()
{
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

void MainWindow::OnClickClearSerialData()
{
    ui->serialDataEdit->clear();
}

void MainWindow::OnClickClearTcpData()
{
    ui->netDataEdit->clear();
}
