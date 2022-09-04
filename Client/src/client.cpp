#include "client.h"

#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QTime>

Client::Client(const QString &str,int port,QWidget *parent)
    : QWidget(parent),BlockNext(0)
{
    socket=new QTcpSocket(this);

    socket->connectToHost(str,port);

    connect(socket,SIGNAL(connected()),this,SLOT(newConnection()));
    connect(socket,SIGNAL(readyRead()),this,SLOT(readReady()));

    QPushButton *cmd=new QPushButton("&send");

    txt_t=new QTextEdit;
    line_t=new QLineEdit;
        
    connect(cmd,SIGNAL(clicked()),this,SLOT(SendToServer()));
    connect(line_t,SIGNAL(returnPressed()),this,SLOT(SendToServer()));

    txt_t->setReadOnly(true);

    QVBoxLayout *layout=new QVBoxLayout;

    layout->addWidget(new QLabel("<H1>Client<H1>"));
    layout->addWidget(txt_t);
    layout->addWidget(line_t);
    layout->addWidget(cmd);


    setLayout(layout);
}

void Client::SendToServer()
{
    QByteArray bit;
    QDataStream out(&bit,QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_2);

    out<<quint16(0)<<QTime::currentTime()<<line_t->text();
    out.device()->seek(0);
    out<<quint16(bit.size()-sizeof(quint16));

    socket->write(bit);
    line_t->setText(" ");
}

void Client::newConnection()
{
    txt_t->append(" ");

}

void Client::readReady()
{
    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_6_2);

    for(;;)
    {
        if(!BlockNext)
        {
            if(socket->bytesAvailable()<sizeof(quint16)){
                break;
            }

            in>>BlockNext;
        }

        if(socket->bytesAvailable()<BlockNext){
            break;
        }

        QTime time;
        QString str;
        in>>time>>str;

        txt_t->append(time.toString()+""+str);

        BlockNext=0;
    }
}
