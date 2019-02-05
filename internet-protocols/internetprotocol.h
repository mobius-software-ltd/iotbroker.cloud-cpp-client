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


public slots:

    virtual void connected();
    virtual void received(char*);
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

static bool matchLineFeed(const QByteArray &pem, int *offset)
{
    char ch = pem.at(*offset);

    // ignore extra whitespace at the end of the line
    while (ch == ' ' && *offset < pem.size())
        ch = pem.at(++*offset);

    if (ch == '\n') {
        *offset += 1;
        return true;
    }
    if (ch == '\r' && pem.size() > (*offset + 1) && pem.at(*offset + 1) == '\n') {
        *offset += 2;
        return true;
    }
    return false;
}

static QByteArray * getKeyFromString(const QByteArray &pem)
{
    int offset = 0;
    QSsl::KeyAlgorithm algo;
    int startPos = pem.indexOf(BEGINKEYSTRING, offset);
    if (startPos != -1)
    {
        algo = QSsl::KeyAlgorithm::Dsa;
    }
    else
    {
        startPos = pem.indexOf(BEGINRSAKEYSTRING, offset);
        if(startPos != -1)
        {
            algo = QSsl::KeyAlgorithm::Rsa;
        }
        else
        {
            return NULL;
        }
    }
    int endPos = 0;
    if(algo == QSsl::KeyAlgorithm::Dsa)
        endPos = pem.indexOf(ENDKEYSTRING, startPos);
    else
        endPos = pem.indexOf(ENDRSAKEYSTRING, startPos);

    if (endPos == -1)
        return NULL;

    if(algo == QSsl::KeyAlgorithm::Dsa) {
        offset = endPos + sizeof(ENDKEYSTRING) - 1;
        endPos+=sizeof(ENDKEYSTRING)-1;
    }
    else {
        offset = endPos + sizeof(ENDRSAKEYSTRING) - 1;
        endPos+=sizeof(ENDRSAKEYSTRING)-1;
    }

//    if (!matchLineFeed(pem, &offset))
//        return NULL;

    QByteArray array = QByteArray::fromRawData(pem.data() + startPos, endPos - startPos);
    return new QByteArray(array,endPos - startPos);

}


#endif // INTERNETPROTOCOL_H
