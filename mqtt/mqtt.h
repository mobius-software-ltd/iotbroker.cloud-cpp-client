#ifndef MQTT_H
#define MQTT_H

#include <QObject>
#include <parser/messagesparser.h>
#include <mqtt/socket/clientsocket.h>
#include <database/accountmanager.h>

/**
 * @brief The MQTT class
 */

class MQTT : public QObject
{
    Q_OBJECT

private:
    ClientSocket *socket;
    MessagesParser *messageParser;

public:
    explicit MQTT(QObject *parent = 0);

    void startTCPConnectionWith(QString &host, quint16 port);
    void stopTCPConnection();

    bool isConnected();

    void sendMessage(Message *message);

    QList<Message *> *decodeMessages(QByteArray data);
    QByteArray encodeMessages(QList<Message *> * messages);

signals:

    void TCPDidStart();
    void messageDidSend(Message *message);
    void messageDidReceive(Message *message);
    void errorMessage(QAbstractSocket::SocketError code);
    void parserError(QString *error);
    void TCPDidDisconnect();

public slots:

    void connectSlot();
    void disconnectSlot();
    void receivedSlot(QByteArray data);
    void errorSlot(QAbstractSocket::SocketError code);
    void messagesParserErrorSlot(QString *error);
};

#endif // MQTT_H
