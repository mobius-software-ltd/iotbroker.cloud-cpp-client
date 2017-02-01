#include "mqtt.h"

MQTT::MQTT(QObject *parent) : QObject(parent)
{
    this->socket = new ClientSocket(parent);
    connect(this->socket, SIGNAL(connect()),                                this, SLOT(connectSlot()));
    connect(this->socket, SIGNAL(received(QByteArray)),                     this, SLOT(receivedSlot(QByteArray)));
    connect(this->socket, SIGNAL(disconnect()),                             this, SLOT(disconnectSlot()));
    connect(this->socket, SIGNAL(withError(QAbstractSocket::SocketError)),  this, SLOT(errorSlot(QAbstractSocket::SocketError)));

    this->messageParser = new MessagesParser(parent);
    connect(this->messageParser, SIGNAL(messagesParserError(QString *)), this, SLOT(messagesParserErrorSlot(QString *)));
}

void MQTT::startTCPConnectionWith(QString &host, quint16 port)
{
    this->socket->connectToHost(host, port);
}

void MQTT::stopTCPConnection()
{
    if (this->socket->isConnect() == true) {
        this->socket->disconnectFromHost();
    }
}

bool MQTT::isConnected()
{
    return this->socket->isConnect();
}

void MQTT::sendMessage(Message *message)
{
    if (this->socket->isConnect() == true) {
        QByteArray byteArray = this->messageParser->encodeMessage(message);
        if (this->socket->sendData(byteArray) == true) {
            emit messageDidSend(message);
        }
    }
}

void MQTT::connectSlot()
{
    emit TCPDidStart();
}

void MQTT::disconnectSlot()
{
    emit TCPDidDisconnect();
}

void MQTT::receivedSlot(QByteArray data)
{
    do {
        QByteArray barray = this->messageParser->nextMessage(data);
        Message *message = this->messageParser->decodeMessage(barray);
        if (message != NULL && message->getType() >= CONNECT && message->getType() <= DISCONNECT) {
            emit messageDidReceive(message);
        }
    } while (data.length() > 0);
}

void MQTT::errorSlot(QAbstractSocket::SocketError code)
{
    emit errorMessage(code);
}

void MQTT::messagesParserErrorSlot(QString *error)
{
    emit parserError(error);
}
