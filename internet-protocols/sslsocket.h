#ifndef SSLSOCKET_H
#define SSLSOCKET_H

#include "internet-protocols/internetprotocol.h"

class SslSocket : public InternetProtocol
{

private:
    QSslSocket *socket;

public:
    SslSocket();
    SslSocket(QString withHost, int port);

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

#endif // SSLSOCKET_H
