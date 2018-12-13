#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "agent.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    SerialAgent *mp_serialAgent;
    TcpAgent *mp_tcpAgent;

private slots:
    void OnSerialDataReady(QByteArray &data);
    void OnTcpDataReady(QByteArray &data);
    void OnClickClearSerialData();
    void OnClickClearTcpData();
};

#endif // MAINWINDOW_H
