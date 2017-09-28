/**
 * Mobius Software LTD
 * Copyright 2015-2017, Mobius Software LTD
 *
 * This is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation; either version 2.1 of
 * the License, or (at your option) any later version.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this software; if not, write to the Free
 * Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301 USA, or see the FSF site: http://www.fsf.org.
 */

#include "mqtt.h"
#include "timer/timersmap.h"
#include "internet-protocols/tcpsocket.h"

MQTT::MQTT(AccountEntity account) : IotProtocol(account)
{
    this->timers = new TimersMap(this);
    this->keepAlive = account.keepAlive;

    this->publishPackets = new QMap<int, Message *>();

    this->messageParser = new MessagesParser(this);

    this->internetProtocol = new TCPSocket(account.serverHost, account.port);

    QObject::connect(this->internetProtocol, SIGNAL(connectionDidStart(InternetProtocol*)),                             this, SLOT(connectionDidStart(InternetProtocol*)));
    QObject::connect(this->internetProtocol, SIGNAL(connectionDidStop(InternetProtocol*)),                              this, SLOT(connectionDidStop(InternetProtocol*)));
    QObject::connect(this->internetProtocol, SIGNAL(didReceiveMessage(InternetProtocol*,QByteArray)),                   this, SLOT(didReceiveMessage(InternetProtocol*,QByteArray)));
    QObject::connect(this->internetProtocol, SIGNAL(didFailWithError(InternetProtocol*,QAbstractSocket::SocketError)),  this, SLOT(didFailWithError(InternetProtocol*,QAbstractSocket::SocketError)));
}

bool MQTT::send(Message *message)
{
    if (this->internetProtocol->getState() == IP_CONNECTION_OPEN) {
        QByteArray data = this->messageParser->encodeMessage(message);
        return this->internetProtocol->send(data);
    }
    return false;
}

void MQTT::goConnect()
{
    QString topicName = this->currentAccount.willTopic.get().toString();
    QByteArray content = this->currentAccount.will.get().toString().toUtf8();
    bool retain = this->currentAccount.isRetain.get().toBool();

    QoS *qos = new QoS(this->currentAccount.qos.get().toInt());
    MQTopic *topic = new MQTopic(topicName, qos);
    Will *will = new Will(topic, content, retain);

    Connect *connect = new Connect();
    connect->setUsername(this->currentAccount.username);
    connect->setPassword(this->currentAccount.password);
    connect->setClientID(this->currentAccount.clientID);
    connect->setCleanSession(this->currentAccount.cleanSession);
    connect->setKeepAlive(this->currentAccount.keepAlive);
    connect->setWill(will);

    this->connect = connect;
    this->timers->goTimeoutTimer();
    this->internetProtocol->start();
}

void MQTT::publish(MessageEntity message)
{
    QString topicName = message.topicName.get().toString();
    QByteArray content = message.content.get().toString().toUtf8();
    int qosNumber = message.qos.get().toInt();

    QoS *qos = new QoS(qosNumber);
    MQTopic *topic = new MQTopic(topicName, qos);

    Publish *publish = new Publish();
    publish->setTopic(topic);
    publish->setContent(content);
    publish->setRetain(message.isRetain.get().toBool());

    if (qosNumber == AT_MOST_ONCE) {
        this->send(publish);
    } else if (qosNumber == AT_LEAST_ONCE || qosNumber == EXACTLY_ONCE) {
        this->timers->goMessageTimer(publish);
    }
}

void MQTT::subscribeTo(TopicEntity topic)
{
    QString topicName = topic.topicName.get().toString();
    QoS *qos = new QoS(topic.qos.get().toInt());
    QList<MQTopic> *topics = new QList<MQTopic>();

    topics->append(MQTopic(topicName, qos));

    Subscribe *subscribe = new Subscribe(topics);

    this->timers->goMessageTimer(subscribe);
}

void MQTT::unsubscribeFrom(TopicEntity topic)
{
    QString topicName = topic.topicName.get().toString();

    QList<QString> *topics = new QList<QString>();
    topics->append(topicName);

    Unsubscribe *unsubscribe = new Unsubscribe(topics);

    this->timers->goMessageTimer(unsubscribe);
}

void MQTT::pingreq()
{
    this->send(new Pingreq());
}

void MQTT::disconnectWith(int duration)
{
    this->send(new Disconnect());
    this->timers->stopAllTimers();
}

Message *MQTT::getPingreqMessage()
{
    return new Pingreq();
}

void MQTT::timeoutMethod()
{
    this->timers->stopAllTimers();
    emit timeout(this);
}

// SLOTS

void MQTT::connectionDidStart(InternetProtocol *protocol)
{
    this->isConnect = true;
    this->timers->goConnectTimer(this->connect);
}

void MQTT::connectionDidStop(InternetProtocol *protocol)
{
    this->isConnect = false;
    this->timers->stopAllTimers();
}

void MQTT::didReceiveMessage(InternetProtocol *protocol, QByteArray data)
{
    do {
        QByteArray barray = this->messageParser->nextMessage(data);
        Message *message = this->messageParser->decodeMessage(barray);

        switch (message->getType()) {
            case MQ_CONNECT:
            {
                emit errorReceived(this, "Packet 'Connect' did receive");
            }
            break;
            case MQ_CONNACK:
            {
                this->timers->stopConnectTimer();
                Connack *connack = (Connack *)message;
                if (connack->getReturnCode() == MQ_ACCEPTED) {
                    this->timers->stopTimeoutTimer();
                    this->timers->goPingTimer(this->keepAlive);
                    emit connackReceived(this, connack->getReturnCode());
                }
            }
            break;
            case MQ_PUBLISH:
            {
                Publish *publish = (Publish *)message;
                int qos = publish->getTopic()->getQoS()->getValue();
                if (qos == AT_LEAST_ONCE) {
                    Puback *puback = new Puback(publish->getPacketID());
                    this->send(puback);
                    emit pubackReceived(this, publish->getTopic()->getName(), publish->getTopic()->getQoS()->getValue(), publish->getContent(), publish->isDup(), publish->isRetain(), 0);
                } else if (qos == EXACTLY_ONCE) {
                    this->publishPackets->insert(publish->getPacketID(), publish);
                    Pubrec *pubrec = new Pubrec(publish->getPacketID());
                    this->timers->goMessageTimer(pubrec);
                }
            }
            break;
            case MQ_PUBACK:
            {
                Puback *puback = (Puback *)message;
                Message *mess = this->timers->stopTimer(puback->getPacketID());
                if (mess->getType() == MQ_PUBLISH) {
                    Publish *publish = (Publish *)mess;
                    emit pubackReceived(this, publish->getTopic()->getName(), publish->getTopic()->getQoS()->getValue(), publish->getContent(), publish->isDup(), publish->isRetain(), 0);
                }
            }
            break;
            case MQ_PUBREC:
            {
                Pubrec *pubrec = (Pubrec *)message;
                Message *mess = this->timers->stopTimer(pubrec->getPacketID());
                if (mess->getType() == MQ_PUBLISH) {
                    Publish *publish = (Publish *)mess;
                    this->publishPackets->insert(publish->getPacketID(), publish);
                    this->timers->goMessageTimer(new Pubrel(publish->getPacketID()));
                }
            }
            break;
            case MQ_PUBREL:
            {
                Pubrel *pubrel = (Pubrel *)message;
                Message *mess = this->timers->stopTimer(pubrel->getPacketID());
                if (mess->getType() == MQ_PUBREC) {
                    Message *publishMessage = this->publishPackets->value(pubrel->getPacketID());
                    if (publishMessage->getType() == MQ_PUBLISH) {
                        Publish *publish = (Publish *)publishMessage;
                        emit pubackReceived(this, publish->getTopic()->getName(), publish->getTopic()->getQoS()->getValue(), publish->getContent(), publish->isDup(), publish->isRetain(), 0);
                    }
                    Pubcomp *pubcomp = new Pubcomp(pubrel->getPacketID());
                    this->send(pubcomp);
                }
            }
            break;
            case MQ_PUBCOMP:
            {
                Pubcomp *pubcomp = (Pubcomp *)message;
                Message *mess = this->timers->stopTimer(pubcomp->getPacketID());
                if (mess->getType() == MQ_PUBREL) {
                    Message *publishMessage = this->publishPackets->value(pubcomp->getPacketID());
                    if (publishMessage->getType() == MQ_PUBLISH) {
                        Publish *publish = (Publish *)publishMessage;
                        emit pubackReceived(this, publish->getTopic()->getName(), publish->getTopic()->getQoS()->getValue(), publish->getContent(), publish->isDup(), publish->isRetain(), 0);
                    }
                }
            }
            break;
            case MQ_SUBSCRIBE:
            {
                emit errorReceived(this, "Packet 'Subscribe' did receive");
            }
            break;
            case MQ_SUBACK:
            {
                Suback *suback = (Suback *)message;
                Message *mess = this->timers->stopTimer(suback->getPacketID());
                if (mess->getType() == MQ_SUBSCRIBE) {
                    Subscribe *subsctibe = (Subscribe *)mess;
                    MQTopic topic = subsctibe->getTopics()->last();
                    emit subackReceived(this, topic.getName(), topic.getQoS()->getValue(), 0);
                }
            }
            break;
            case MQ_UNSUBSCRIBE:
            {
                emit errorReceived(this, "Packet 'Unsubscribe' did receive");
            }
            break;
            case MQ_UNSUBACK:
            {
                Unsuback *unsuback = (Unsuback *)message;
                Message *mess = this->timers->stopTimer(unsuback->getPacketID());
                if (mess->getType() == MQ_UNSUBSCRIBE) {
                    Unsubscribe *unsubscribe = (Unsubscribe *)mess;
                    emit unsubackReceived(this, unsubscribe->getTopics()->last());
                }
            }
            break;
            case MQ_PINGREQ:
            {
                emit errorReceived(this, "Packet 'Pingreq' did receive");
            }
            break;
            case MQ_PINGRESP:
            {
                emit pingrespReceived(this);
            }
            break;
            case MQ_DISCONNECT:
            {
                this->timers->stopAllTimers();
                emit disconnectReceived(this);
            }
            break;
        }

    } while (data.length() > 0);
}

void MQTT::didFailWithError(InternetProtocol *protocol, QAbstractSocket::SocketError error)
{
    switch (error) {
        case QAbstractSocket::ConnectionRefusedError:            emit errorReceived(this, QString("connection refused error"));            break;
        case QAbstractSocket::RemoteHostClosedError:             emit errorReceived(this, QString("remote host closed error"));            break;
        case QAbstractSocket::HostNotFoundError:                 emit errorReceived(this, QString("host not found error"));                break;
        case QAbstractSocket::SocketAccessError:                 emit errorReceived(this, QString("socket access error"));                 break;
        case QAbstractSocket::SocketResourceError:               emit errorReceived(this, QString("socket resource error"));               break;
        case QAbstractSocket::SocketTimeoutError:                emit errorReceived(this, QString("socket timeout error"));                break;
        case QAbstractSocket::DatagramTooLargeError:             emit errorReceived(this, QString("datagram too large error"));            break;
        case QAbstractSocket::NetworkError:                      emit errorReceived(this, QString("network error"));                       break;
        case QAbstractSocket::AddressInUseError:                 emit errorReceived(this, QString("address in use error"));                break;
        case QAbstractSocket::SocketAddressNotAvailableError:    emit errorReceived(this, QString("socket address not available error"));  break;
        case QAbstractSocket::UnsupportedSocketOperationError:   emit errorReceived(this, QString("unsupported socket operation error"));  break;
        case QAbstractSocket::UnfinishedSocketOperationError:    emit errorReceived(this, QString("unfinished socket operation error"));   break;
        case QAbstractSocket::ProxyAuthenticationRequiredError:  emit errorReceived(this, QString("proxy authentication required error")); break;
        case QAbstractSocket::SslHandshakeFailedError:           emit errorReceived(this, QString("ssl handshake failed error"));          break;
        case QAbstractSocket::ProxyConnectionRefusedError:       emit errorReceived(this, QString("proxy connection refused error"));      break;
        case QAbstractSocket::ProxyConnectionClosedError:        emit errorReceived(this, QString("proxy connection closed error"));       break;
        case QAbstractSocket::ProxyConnectionTimeoutError:       emit errorReceived(this, QString("proxy connection timeout error"));      break;
        case QAbstractSocket::ProxyNotFoundError:                emit errorReceived(this, QString("proxy not found error"));               break;
        case QAbstractSocket::ProxyProtocolError:                emit errorReceived(this, QString("proxy protocol error"));                break;
        case QAbstractSocket::OperationError:                    emit errorReceived(this, QString("operation error"));                     break;
        case QAbstractSocket::SslInternalError:                  emit errorReceived(this, QString("ssl internal error"));                  break;
        case QAbstractSocket::SslInvalidUserDataError:           emit errorReceived(this, QString("ssl invalid user data error"));         break;
        case QAbstractSocket::TemporaryError:                    emit errorReceived(this, QString("temporary error"));                     break;
        case QAbstractSocket::UnknownSocketError:                emit errorReceived(this, QString("unknown socket error"));                break;
    }
}

void MQTT::parseFailWithError(QString *error)
{
    emit errorReceived(this, *error);
}
