#ifndef TCPSOCKET_H
#define TCPSOCKET_H

#include "internet-protocols/internetprotocol.h"

class TCPSocket : public InternetProtocol
{

private:
    QTcpSocket *socket;

public:
    TCPSocket();
    TCPSocket(QString withHost, int andPort);

    virtual bool setCertificate(QString path, QString pass);

    virtual void start();
    virtual void stop();
    virtual bool send(QByteArray data);

public slots:

    virtual void readyRead();
    virtual void disconnected();
    virtual void error(QAbstractSocket::SocketError error);
    virtual void stateDidChanged(QAbstractSocket::SocketState state);
};

#endif // TCPSOCKET_H
