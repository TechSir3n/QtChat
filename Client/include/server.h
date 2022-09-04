#pragma once

#include <QWidget>
#include <QTcpSocket>
#include <QLineEdit>
#include <QTextEdit>



class Client : public QWidget
{
    Q_OBJECT

public:
    Client(const QString &str,int port,QWidget *parent = nullptr);

private:
    QTcpSocket *socket;
    QTextEdit *txt_t;
    QLineEdit *line_t;
    quint16 BlockNext;

public slots:
    void SendToServer();
    void newConnection();
    void readReady();
};
