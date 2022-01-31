#ifndef NAME_H
#define NAME_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QJsonDocument>
#include<QJsonObject>
#include <QJsonParseError>
#include <QFile>

#include <QSqlDatabase>
#include <QSqlQuery>

class myserver : public QTcpServer
{
    Q_OBJECT
public:
    myserver();
    ~myserver();

    QTcpSocket* socket;
    QByteArray Data;

    QTcpSocket doc;
    QJsonParseError docError;

    QSqlDatabase db;
public slots:
    void startServer();
    void incomingConnection(int socketDescriptor);
    void sockReady();
    void sockDisc();
};

#endif // NAME_H





