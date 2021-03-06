#ifndef INTERNETPROTOCOL_H
#define INTERNETPROTOCOL_H

#include <QString>
#include <QObject>
#include <QSslSocket>
#include <QTcpSocket>
#include <QUdpSocket>
#include <QSslKey>

#define BEGINKEYSTRING "-----BEGIN PRIVATE KEY-----"
#define ENDKEYSTRING "-----END PRIVATE KEY-----"
#define BEGINDSAKEYSTRING "-----BEGIN DSA PRIVATE KEY-----"
#define ENDDSAKEYSTRING "-----END DSA PRIVATE KEY-----"
#define BEGINRSAKEYSTRING "-----BEGIN RSA PRIVATE KEY-----"
#define ENDRSAKEYSTRING "-----END RSA PRIVATE KEY-----"

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

    virtual bool setCertificate(QString path, QString pass) = 0;

    virtual void start() = 0;
    virtual void stop() = 0;
    virtual bool send(QByteArray data) = 0;

    ConnectionState getState() const;
    void setState(const ConnectionState &value);

    QString getHost() const;
    void setHost(const QString &value);

    int getPort() const;
    void setPort(int value);

    static QString socketErrorToString(QAbstractSocket::SocketError error);
    static QByteArray * getKeyFromString(const QByteArray &pem);


public slots:

    virtual void connected();
    virtual void received(char*);
    virtual void received(char*, int size);
    virtual void errorString(char *);

    virtual void receivedByteArray(QByteArray);
    virtual void receivedString(QString);

    virtual void readyRead() = 0;
    virtual void disconnected() = 0;
    virtual void error(QAbstractSocket::SocketError error) = 0;
    virtual void stateDidChanged(QAbstractSocket::SocketState state) = 0;

signals:

    void connectionDidStart(InternetProtocol *protocol);
    void connectionDidStop(InternetProtocol *protocol);
    void didReceiveMessage(InternetProtocol *protocol, QByteArray data);
    void didFailWithError(InternetProtocol *protocol, QString error);

};

#endif // INTERNETPROTOCOL_H
