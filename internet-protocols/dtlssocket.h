#ifndef DTLSSOCKET_H
#define DTLSSOCKET_H

#include <QtConcurrent/QtConcurrent>
#include "internet-protocols/internetprotocol.h"
#include "classes/dtls.h"

class DtlsSocket : public InternetProtocol
{

private:
    Dtls *socket;
    QFuture<void> future;

public:
    DtlsSocket();
    DtlsSocket(QString withHost, int port);

    virtual bool setCertificate(QString path, QString pass);

    virtual void start();
    virtual void stop();
    virtual bool send(QByteArray data);

public slots:

    void connected();
    void received(char *message);
    virtual void errorString(char *message);

    virtual void readyRead();
    virtual void disconnected();
    virtual void error(QAbstractSocket::SocketError error);
    virtual void stateDidChanged(QAbstractSocket::SocketState state);
};

#endif // DTLSSOCKET_H
