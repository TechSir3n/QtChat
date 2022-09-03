#pragma once

#include <QWidget>
#include <QTcpServer>
#include <QTcpSocket>
#include <QTextEdit>



class Server : public QWidget
{
    Q_OBJECT

public:
    Server(int port,QWidget *parent = nullptr);

 private:
   QTcpServer *server;
   QTextEdit *txt_t;
   quint16 BlockNext;

private:
   void SentToClient(QTcpSocket *m_socket,const QString &str);

public slots:
   void newConnection();
   void readReady();
};

