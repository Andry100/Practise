#include "name.h"

myserver::myserver(){}

myserver::~myserver(){}

void myserver::startServer()
{
    if(this->listen(QHostAddress::Any, 4423))
    {
        db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName("place located data");
        if(db.open())
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

    socket->write("{\"type\":\"connect\",\"status\":\"yes\"}");
    qDebug()<<"Send client connect status - Yes";
}

void myserver::sockReady()
{   
    Data = socket->readAll();
    qDebug()<<"Data:" << Data;

    QJsonDocument doc = QJsonDocument::fromJson(Data, &docError);

    if(docError.errorString().toInt()==QJsonParseError::NoError)
    {
        if((doc.object().value("type").toString()=="select") && (doc.object().value("params").toString() == "workers"))
        {
            if(db.isOpen())
            {
                QByteArray itog = "{\"type\":\"resultSelect\",\"result\":[";

                QSqlQuery* query = new  QSqlQuery(db);
                if(query->exec("SELECT name FROM listflats"))
                {
                    while(query->next())
                    {
                        itog.append("{\"name\":\""+query->value(0).toString()+"\"},");
                    }
                    itog.remove(itog.length() -1,1);
                    itog.append("]}");

                    socket->write(itog);
                    socket->waitForBytesWritten(500);
                }
            }
           QFile file;
           file.setFileName("  ");
          if(file.open(QIODevice::ReadOnly|QFile::Text))
          {
              QByteArray fromFile = file.readAll();
              QByteArray itog = "{\"type\":\"resultSelect\",\"result\":"+fromFile+"}";

              socket->write(itog);
              socket->waitForBytesWritten(500);
          }
          file.close();
        }
    }
}

void myserver::sockDisc()
{
    qDebug()<<"Disconnect";
    socket->deleteLater();
}










