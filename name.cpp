#include "name.h"

myserver::myserver(){}

myserver::~myserver(){}

void myserver::startServer()
{
    if(this->listen(QHostAddress::Any, 4423))
    {
        db = QSqlDatabase::addDatabase("QSQLITE");
        db1 = QSqlDatabase::addDatabase("QSQSECOND");
        db.setDatabaseName("place located data");
        db1.setDatabaseName("place located data two");
        if(db.open() && db1.open())
        {
        qDebug()<< " DB open ";
        }
        else
        {
            qDebug()<< "DB not open";
        }
    }
    else
    {
        qDebug()<<"Not listening";
    }
}

void myserver::incomingConnection(int socketDescriptor)
{
    socket = new QTcpSocket(this);
    socket -> setSocketDescriptor(socketDescriptor);

      connect(socket,SIGNAL(readyRead()),this,SLOT(sockReady()));
      connect(socket,SIGNAL(disconnected()),this,SLOT(sockDisc()));

    qDebug()<<socketDescriptor<<"Client connected";

    socket->write("{\type\":\"connect\",\"status\":\"yes\"}");
    qDebug()<<"Send client connect status - Yes";
}

void myserver::sockReady()
{
Data = socket->readAll();
qDebug()<<"Data:" << Data;

doc = QJsonDocument::fromJson(Data, &docError);

if(docError.errorString().toInt()==QJsonParseError::NoError)
{
    if((doc.object().value("type").toString()=="recSize") && (doc.object().value("params").toString() == "flats"))
    {
        itog = "{\"type\":\"resultSelect\",\"result\":[";
        it = "{\"type\":\"resultSelect\",\"result\":[";

        if(db.isOpen() && db1.isOpen())
        {

             QSqlQuery* query = new QSqlQuery(db);
             QSqlQuery* query = new QSqlQuery(db1);
            if(query->exec("SELECT name FROM listflats"))
            {
                query1->exec("Select name FROM lictcomments");
                while(query->next() && query1->next())
                {
                    if(itog.append("{\"name\":\""+query->value(0).toString()+"\"},"))
                    {
                        it.append("{\"name\":\""+query1->value(0).toString()+"\"},";
                    }

                }
                it.remove(it.lenght()-1,1);
                itog.remove(itog.lenght()-1,1);
            }
            else
            {
                eDebug()<<"Querry not sucsses";
                eDebug()<<"Querrq1 not sucsses";
            }
            delete query;
            delete query1;
        }
      itog.append("}}");
      it.append("}}");

      socket->write("{\"type\":\"size\",\"resp\":\"flats\:",\"lenght\":"+QByteArray::number(itog.size())+"}");
      socket->waitForBytesWritten(500);
      socket->write("{\"type\":\"size\",\"resp\":\"coments\:",\"lenght\":"+QByteArray::number(it.size())+"}");
      socket->waitForBytesWritten(500);
    }

    else if((doc.object().value("type").toString()=="recSize") && (doc.object().value("params").toString() == "flats"))
    {
        socket->write(itog);
        qDebug()<<"Size messege"<<socket->bytesToWrite();
        socket->waitForBytesWritten(500);
    }
}
}

void nameSockDisc()
{
    qDebug()<<"Disconnect";
    socket->deleteLater();
}










