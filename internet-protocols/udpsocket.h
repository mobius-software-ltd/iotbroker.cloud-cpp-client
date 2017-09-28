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

#endif // UDPSOCKET_H
