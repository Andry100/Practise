#ifndef NAME_H
#define NAME_H

#include <QTcpServer>
#include <QTcpSocket>

class myserver : public QTcpServer
{
    Q_OBJECT
public:
    myserver();
    ~myserver();

    QTcpSocket* socket;
    QByteArray Data;

public slots:
    void startServer();
    void incomingConnection(int socketDescriptor);
    void sockReady();
    void sockDisc();
};

#endif // NAME_H
