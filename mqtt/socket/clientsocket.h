#ifndef CLIENTSOCKET_H
#define CLIENTSOCKET_H

#include <QObject>
#include <QTcpSocket>
#include <QByteArray>
#include <QDebug>
#include <QNetworkProxy>
#include <QAuthenticator>

/**
 * @brief The ClientSocket class
 */

class ClientSocket : public QObject
{
    Q_OBJECT

private:
    QTcpSocket *socket;

public:
    explicit ClientSocket(QObject *parent = 0);

    void connectToHost(QString &host, quint16 port);
    bool sendData(QByteArray data);
    void disconnectFromHost();

    bool isConnect();

public slots:

    void aboutToCloseSlot();
    void bytesWrittenSlot(qint64 bytes);
    void channelBytesWrittenSlot(int channel, qint64 bytes);
    void channelReadyReadSlot(int channel);
    void destroyedSlot(QObject *object);
    void disconnectedSlot();
    void errorSlot(QAbstractSocket::SocketError error);
    void hostDidFoundSlot();
    void objectNameChangedSlot(QString objectName);
    void readChannelFinishedSlot();
    void readyReadSlot();
    void stateDidChangedSlot(QAbstractSocket::SocketState state);

signals:

    void connect();
    void withError(QAbstractSocket::SocketError error);
    void received(QByteArray data);
    void disconnect();
};

#endif // CLIENTSOCKET_H
