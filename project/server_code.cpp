#include "name.h"

name::name(){}

name::~name(){}

void name::startServer()
{
    if(this->listen(QHostAddress::Any, 4423))
    {
        eDebug()<<"Listening";
    }
    else
    {
        eDebug()<<"Not listening";
    }
}

void myserver::incomingConnection(int socketDescriptor)
{
    socket = new QTcpSocket(this);
    socket -> setSocketDescriptor(socketDescriptor);

      connect(socket,SIGNAL(readyRead()),this,SLOT(sockReady()));
      connect(socket,SIGNAL(disconnected()),this,SLOT(sockDisc()));

    qDebug()<<socketDescriptor<<"Client connected";

    socket->write("You are connect");
    qDebug()<<"Send client connect status - Yes";
}

void nameSockReady()
{

}

void nameSockDisc()
{
    qDebug()<<"Disconnect";
    socket->deleteLater();
}















