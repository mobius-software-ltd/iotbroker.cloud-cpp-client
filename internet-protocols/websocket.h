#ifndef WEBSOCKET_H
#define WEBSOCKET_H

#include "internet-protocols/internetprotocol.h"
#include <QtWebSockets/QWebSocket>

class WebSocket : public InternetProtocol
{

private:
    QWebSocket *socket;

public:
    WebSocket();
    WebSocket(QString withHost, int andPort);

    virtual bool setCertificate(QString path, QString pass);

    virtual void start();
    virtual void stop();
    virtual bool send(QByteArray data);

public slots:

    void receivedByteArray(QByteArray);

    virtual void readyRead();
    virtual void disconnected();
    virtual void error(QAbstractSocket::SocketError error);
    virtual void stateDidChanged(QAbstractSocket::SocketState state);

};

#endif // WEBSOCKET_H
