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
    SslSocket(QString withHost, int port, QString certificatePath, QString certificatePass);

    virtual void start();
    virtual void stop();
    virtual bool send(QByteArray data);

public slots:

    //void aboutToCloseSlot();
    //void bytesWrittenSlot(qint64 bytes);
    //void channelBytesWrittenSlot(int channel, qint64 bytes);
    virtual void readyRead();
    //void destroyedSlot(QObject *object);
    virtual void disconnected();
    virtual void error(QAbstractSocket::SocketError error);
    //void hostDidFoundSlot();
    //void objectNameChangedSlot(QString objectName);
    //void readChannelFinishedSlot();
    //void readyReadSlot();
    virtual void stateDidChanged(QAbstractSocket::SocketState state);

};

#endif // SSLSOCKET_H
