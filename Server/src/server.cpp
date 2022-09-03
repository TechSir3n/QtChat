#include "server.h"

#include <QMessageBox>
#include <QVBoxLayout>
#include <QLabel>
#include <QTime>

Server::Server(int port,QWidget *parent) : QWidget(parent),BlockNext(0)
{
   server=new QTcpServer(this);

   if(!server->listen(QHostAddress::Any,port)){
       QMessageBox::critical(0,"Here will error",",Can't connecting");

       server->close();
   }

   connect(server,SIGNAL(newConnection()),this,SLOT(newConnection()));

   txt_t=new QTextEdit;

   txt_t->setReadOnly(true);

   QVBoxLayout *layout=new QVBoxLayout;

   layout->addWidget(new QLabel("<H1>Server<H1>"));
   layout->addWidget(txt_t);

   setLayout(layout);
}

void Server::SentToClient(QTcpSocket *m_socket, const QString &str)
{
    QByteArray bit;
    QDataStream out(&bit,QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_2);

    out << quint16(0)<<QTime::currentTime() << str;
    out.device()->seek(0);
    out<<quint16(bit.size()-sizeof(quint16));

    m_socket->write(bit);
}

void Server::newConnection()
{
    QTcpSocket *socket=server->nextPendingConnection();

    connect(socket,SIGNAL(disconnected()),socket,SLOT(deleteLater()));
    connect(socket,SIGNAL(readyRead()),this,SLOT(readReady()));

    SentToClient(socket," Welcome Client  ");
}

void Server::readReady()
{
    QTcpSocket *ptSocket=(QTcpSocket*)sender();
    QDataStream in(ptSocket);

    in.setVersion(QDataStream::Qt_6_2);

    for(;;)
    {
        if(!BlockNext)
        {
           if(ptSocket->bytesAvailable()<sizeof(quint16))
           {
               break;
           }

         in>>BlockNext;

        }

        if(ptSocket->bytesAvailable()<BlockNext){
            break;
        }

        QTime time;
        QString str;

        in>>time>>str;

        QString message=time.toString()+" " + " Message -> " + str;

        txt_t->append(message);

        BlockNext=0;

        SentToClient(ptSocket," -> " + str);
     }
}


