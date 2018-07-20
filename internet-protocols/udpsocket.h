#ifndef UDPSOCKET_H
#define UDPSOCKET_H

#include "internet-protocols/internetprotocol.h"

class UDPSocket : public InternetProtocol
{

private:
    QUdpSocket *socket;

public:
    UDPSocket();
    UDPSocket(QString withHost, int andPort);

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

#endif // UDPSOCKET_H
