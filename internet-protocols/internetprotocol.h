#ifndef INTERNETPROTOCOL_H
#define INTERNETPROTOCOL_H

#include <QString>
#include <QObject>
#include <QTcpSocket>
#include <QUdpSocket>

enum ConnectionState
{
    IP_CONNECTION_OPENING = 0,
    IP_CONNECTION_OPEN = 1,
    IP_CONNECTION_CLOSING = 2,
    IP_CONNECTION_CLOSED = 3
};

class InternetProtocol : public QObject
{
    Q_OBJECT

    ConnectionState state;
    QString host;
    int port;

public:
    InternetProtocol();
    InternetProtocol(QString withHost, int andPort);

    virtual void start() = 0;
    virtual void stop() = 0;
    virtual bool send(QByteArray data) = 0;

    ConnectionState getState() const;
    void setState(const ConnectionState &value);

    QString getHost() const;
    void setHost(const QString &value);

    int getPort() const;
    void setPort(int value);


public slots:

    virtual void readyRead() = 0;
    virtual void disconnected() = 0;
    virtual void error(QAbstractSocket::SocketError error) = 0;
    virtual void stateDidChanged(QAbstractSocket::SocketState state) = 0;

signals:

    void connectionDidStart(InternetProtocol *protocol);
    void connectionDidStop(InternetProtocol *protocol);
    void didReceiveMessage(InternetProtocol *protocol, QByteArray data);
    void didFailWithError(InternetProtocol *protocol, QAbstractSocket::SocketError error);

};

#endif // INTERNETPROTOCOL_H
