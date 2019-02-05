#ifndef SSLWEBSOCKET_H
#define SSLWEBSOCKET_H

#include "internet-protocols/internetprotocol.h"
#include <QtWebSockets/QWebSocket>

class SslWebSocket : public InternetProtocol
{

private:
    QWebSocket *socket;

public:
    SslWebSocket();
    SslWebSocket(QString withHost, int andPort);

    virtual bool setCertificate(QString path, QString pass);

    virtual void start();
    virtual void stop();
    virtual bool send(QByteArray data);

public slots:

    void receivedByteArray(QByteArray);
    void receivedString(QString);

    virtual void readyRead();
    virtual void disconnected();
    virtual void error(QAbstractSocket::SocketError error);
    virtual void stateDidChanged(QAbstractSocket::SocketState state);

};

#endif // SSLWEBSOCKET_H
