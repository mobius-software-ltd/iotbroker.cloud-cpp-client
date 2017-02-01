#include "mqttmanager.h"

MQTTManager::MQTTManager(QObject *parent) : QObject(parent)
{
    this->mqtt = new MQTT(parent);
    this->accountManager = AccountManager::getInstance();

    connect(this->mqtt, SIGNAL(TCPDidStart()),                              this, SLOT(TCPDidStartSlot()));
    connect(this->mqtt, SIGNAL(messageDidSend(Message *)),                  this, SLOT(messageDidSendSlot(Message *)));
    connect(this->mqtt, SIGNAL(messageDidReceive(Message *)),               this, SLOT(messageDidReceiveSlot(Message *)));
    connect(this->mqtt, SIGNAL(errorMessage(QAbstractSocket::SocketError)), this, SLOT(errorMessageSlot(QAbstractSocket::SocketError)));
    connect(this->mqtt, SIGNAL(parserError(QString *)),                     this, SLOT(parserErrorSlot(QString *)));
    connect(this->mqtt, SIGNAL(TCPDidDisconnect()),                         this, SLOT(TCPDidDisconnectSlot()));
}

void MQTTManager::openConnectionWith(QString &host, quint16 port)
{
    this->mqtt->startTCPConnectionWith(host, port);
}

bool MQTTManager::connectMessage(Connect *connect)
{
    this->mqtt->sendMessage(connect);
}

bool MQTTManager::publishMessage(Publish *publish)
{
    this->mqtt->sendMessage(publish);
}

bool MQTTManager::pubackMessage(Puback *puback)
{
    this->mqtt->sendMessage(puback);
}

bool MQTTManager::pubrecMessage(Pubrec *pubrec)
{
    this->mqtt->sendMessage(pubrec);
}

bool MQTTManager::pubrelMessage(Pubrel *pubrel)
{
    this->mqtt->sendMessage(pubrel);
}

bool MQTTManager::pubcompMessage(Pubcomp *pubcomp)
{
    this->mqtt->sendMessage(pubcomp);
}

bool MQTTManager::subscribeMessage(Subscribe *subscribe)
{
    this->mqtt->sendMessage(subscribe);
}

bool MQTTManager::unsubscribeMessage(Unsubscribe *unsubscribe)
{
    this->mqtt->sendMessage(unsubscribe);
}

bool MQTTManager::pingreqMessage()
{
    this->mqtt->sendMessage(new Pingreq);
}

bool MQTTManager::disconnectMessage()
{
    this->mqtt->sendMessage(new Disconnect());
}

MQTT *MQTTManager::getMQTT()
{
    return this->mqtt;
}

// SLOTS

void MQTTManager::TCPDidStartSlot()
{
    emit openConnection();
}

void MQTTManager::messageDidSendSlot(Message *message)
{

}

void MQTTManager::messageDidReceiveSlot(Message *message)
{
    switch (message->getType()) {
        case CONNECT:       emit errorMessageDidReceive(new QString("DID RECEIVE CONNECT MESSAGE"));        break;
        case CONNACK:       emit connackDidReceive((Connack *)message);                                     break;
        case PUBLISH:       emit publishDidReceive((Publish *)message);                                     break;
        case PUBACK:        emit pubackDidReceive((Puback *)message);                                       break;
        case PUBREC:        emit pubrecDidReceive((Pubrec *)message);                                       break;
        case PUBREL:        emit pubrelDidReceive((Pubrel *)message);                                       break;
        case PUBCOMP:       emit pubcompDidReceive((Pubcomp *)message);                                     break;
        case SUBSCRIBE:     emit errorMessageDidReceive(new QString("DID RECEIVE SUBSCRIBE MESSAGE"));      break;
        case SUBACK:        emit subackDidReceive((Suback *)message);                                       break;
        case UNSUBSCRIBE:   emit errorMessageDidReceive(new QString("DID RECEIVE UNSUBSCRIBE MESSAGE"));    break;
        case UNSUBACK:      emit unsubackDidReceive((Unsuback *)message);                                   break;
        case PINGREQ:       emit errorMessageDidReceive(new QString("DID RECEIVE PINGREQ MESSAGE"));        break;
        case PINGRESP:      emit pingrespDidReceive((Pingresp *)message);                                   break;
        case DISCONNECT:    emit errorMessageDidReceive(new QString("DID RECEIVE DISCONNECT MESSAGE"));     break;
        default:            emit errorMessageDidReceive(new QString("DID RECEIVE UNKNOWN MESSAGE"));        break;
    }
}

void MQTTManager::errorMessageSlot(QAbstractSocket::SocketError code)
{
    emit errorString(new QString("QAbstractSocket::SocketError = " + QString::number(code)));
}

void MQTTManager::parserErrorSlot(QString *error)
{
    emit errorString(error);
}

void MQTTManager::TCPDidDisconnectSlot()
{
    emit closeConnection();
}
