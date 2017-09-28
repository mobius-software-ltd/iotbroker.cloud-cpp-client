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

#include "mqttsn.h"
#include "internet-protocols/udpsocket.h"
#include "iot-protocols/mqtt-sn/classes/topics/snidentifiertopic.h"

MqttSN::MqttSN(AccountEntity account) : IotProtocol(account)
{
    this->timers = new TimersMap(this);
    this->keepAlive = account.keepAlive;

    this->messageParser = new SNMessagesParser(this);
    this->publishPackets = new QMap<int, Message *>();
    this->publishObj = NULL;

    this->internetProtocol = new UDPSocket(account.serverHost, account.port);

    QObject::connect(this->internetProtocol, SIGNAL(connectionDidStart(InternetProtocol*)),                             this, SLOT(connectionDidStart(InternetProtocol*)));
    QObject::connect(this->internetProtocol, SIGNAL(connectionDidStop(InternetProtocol*)),                              this, SLOT(connectionDidStop(InternetProtocol*)));
    QObject::connect(this->internetProtocol, SIGNAL(didReceiveMessage(InternetProtocol*,QByteArray)),                   this, SLOT(didReceiveMessage(InternetProtocol*,QByteArray)));
    QObject::connect(this->internetProtocol, SIGNAL(didFailWithError(InternetProtocol*,QAbstractSocket::SocketError)),  this, SLOT(didFailWithError(InternetProtocol*,QAbstractSocket::SocketError)));

    QObject::connect(this->messageParser, SIGNAL(messagesParserError(QString*)), this, SLOT(parseFailWithError(QString*)));
}

bool MqttSN::send(Message *message)
{
    if (this->internetProtocol->getState() == IP_CONNECTION_OPEN) {
        QByteArray data = this->messageParser->encodeMessage(message);
        return this->internetProtocol->send(data);
    }
    return false;
}

void MqttSN::goConnect()
{
    SNConnect *connect = new SNConnect();
    connect->setWillPresent(this->currentAccount.willTopic.get().toString().length() != 0);
    connect->setCleanSession(this->currentAccount.cleanSession.get().toBool());
    connect->setDuration(this->currentAccount.keepAlive.get().toInt());
    connect->setClientID(this->currentAccount.clientID.get().toString());

    this->connect = connect;
    this->timers->goTimeoutTimer();
    this->internetProtocol->start();
}

void MqttSN::publish(MessageEntity message)
{
    QString topicName = message.topicName.get().toString();
    QoS *qos = new QoS(message.qos.get().toInt());
    QByteArray *content = new QByteArray(message.content.get().toByteArray());
    bool isDup = message.isDub.get().toBool();
    bool isRetain = message.isRetain.get().toBool();

    SNRegister *registerPacket = new SNRegister(0, 0, topicName);
    Topic *topic = new SNFullTopic(topicName, qos);

    this->publishObj = new SNPublish(0, topic, content, isDup, isRetain);

    this->timers->goRegisterTimer(registerPacket);
}

void MqttSN::subscribeTo(TopicEntity topic)
{
    QString topicName = topic.topicName.get().toString();
    QoS *qos = new QoS(topic.qos.get().toInt());
    Topic *topicObj = new SNFullTopic(topicName, qos);

    SNSubscribe *subscribe = new SNSubscribe(0, topicObj, false);
    this->timers->goMessageTimer(subscribe);
}

void MqttSN::unsubscribeFrom(TopicEntity topic)
{
    QString topicName = topic.topicName.get().toString();
    QoS *qos = new QoS(topic.qos.get().toInt());
    Topic *topicObj = new SNFullTopic(topicName, qos);

    SNUnsubscribe *unsubscribe = new SNUnsubscribe(0, topicObj);
    this->timers->goMessageTimer(unsubscribe);
}

void MqttSN::pingreq()
{
    this->send(new SNPingreq());
}

void MqttSN::disconnectWith(int duration)
{
    this->send(new SNDisconnect(duration));
    this->timers->stopAllTimers();
}

Message *MqttSN::getPingreqMessage()
{
    return new SNPingreq();
}

void MqttSN::timeoutMethod()
{
    this->isConnect = false;
    this->timers->stopAllTimers();
    emit timeout(this);
}

// SLOTS

void MqttSN::connectionDidStart(InternetProtocol *protocol)
{
    this->isConnect = true;
    this->timers->goConnectTimer(this->connect);
}

void MqttSN::connectionDidStop(InternetProtocol *protocol)
{
    this->isConnect = false;
    this->timers->stopAllTimers();
}

void MqttSN::didReceiveMessage(InternetProtocol *protocol, QByteArray data)
{
    Message *message = this->messageParser->decodeMessage(data);

    switch(message->getType()) {
        case SN_ADVERTISE:
        {
            emit errorReceived(this, "Packet 'Advertise' did receive");
        } break;
        case SN_SEARCHGW:
        {
            emit errorReceived(this, "Packet 'Search' did receive");
        } break;
        case SN_GWINFO:
        {
            emit errorReceived(this, "Packet 'GWInfo' did receive");
        } break;
        case SN_CONNECT:
        {
            emit errorReceived(this, "Packet 'Connect' did receive");
        } break;
        case SN_CONNACK:
        {
            this->isConnect = true;
            SNConnack *connack = (SNConnack *)message;
            this->timers->stopConnectTimer();
            emit connackReceived(this, connack->getReturnCode());
            this->timers->goPingTimer(this->currentAccount.keepAlive.get().toInt());
        } break;
        case SN_WILLTOPICREQ:
        {
            this->timers->stopConnectTimer();

            QString topicName = this->currentAccount.willTopic.get().toString();
            QoS *qos = new QoS(this->currentAccount.qos.get().toInt());
            bool isRetain = this->currentAccount.isRetain.get().toInt();

            SNFullTopic *fullTopic = new SNFullTopic(topicName, qos);
            SNWillTopic *topic = new SNWillTopic(fullTopic, isRetain);
            this->send(topic);
        } break;
        case SN_WILLTOPIC:
        {
            emit errorReceived(this, "Packet 'Will topic' did receive");
        } break;
        case SN_WILLMSGREQ:
        {
            SNWillMsg *willMsgReq = new SNWillMsg(this->currentAccount.will.get().toString().toUtf8());
            this->send(willMsgReq);
        } break;
        case SN_WILLMSG:
        {
            emit errorReceived(this, "Packet 'Will msg' did receive");
        } break;
        case SN_REGISTER:
        {
            SNRegister *reg = (SNRegister *)message;
            SNRegack *regack = new SNRegack(reg->getTopicID(), reg->getPacketID(), SN_ACCEPTED_RETURN_CODE);
            this->send(regack);
        } break;
        case SN_REGACK:
        {
            SNRegack *regack = (SNRegack *)message;
            this->timers->stopRegisterTimer();

            if (regack->getCode() == SN_ACCEPTED_RETURN_CODE) {
                SNIdentifierTopic *topic = new SNIdentifierTopic(regack->getTopicID(), this->publishObj->getTopic()->getQoS());
                this->publishObj->setPacketID(regack->getPacketID());
                this->publishObj->setTopic(topic);
                if (this->publishObj->getTopic()->getQoS()->getValue() == AT_MOST_ONCE) {
                    this->send(this->publishObj);
                } else {
                    this->timers->goMessageTimer(this->publishObj);
                }
            }

        } break;
        case SN_PUBLISH:
        {
            SNPublish *publish = (SNPublish *)message;

            if (publish->getTopic()->getQoS()->getValue() == AT_LEAST_ONCE) {
                QString topicIDString = QString(publish->getTopic()->encode());
                int topicID = topicIDString.toInt();
                SNPuback *puback = new SNPuback(topicID, publish->getPacketID(), SN_ACCEPTED_RETURN_CODE);
                this->send(puback);
            } else if (publish->getTopic()->getQoS()->getValue() == EXACTLY_ONCE) {
                SNPubrec *pubrec = new SNPubrec(publish->getPacketID());
                this->publishPackets->insert(publish->getPacketID(), publish);
                this->timers->goMessageTimer(pubrec);
            }
            QString topicName = QString(publish->getTopic()->encode());
            emit publishReceived(this, topicName, publish->getTopic()->getQoS()->getValue(), *publish->getContent(), publish->getIsDup(), publish->getIsRetain());
        } break;
        case SN_PUBACK:
        {
            SNPuback *puback = (SNPuback *)message;
            SNPublish *publish = (SNPublish *)this->timers->removeTimer(puback->getPacketID());
            QString topicName = QString(publish->getTopic()->encode());
            emit pubackReceived(this, topicName, publish->getTopic()->getQoS()->getValue(), *publish->getContent(), publish->getIsDup(), publish->getIsRetain(), puback->getCode());
            this->publishPackets->remove(publish->getPacketID());
        } break;
        case SN_PUBCOMP:
        {
            SNPubcomp *pubcomp = (SNPubcomp *)message;
            this->timers->removeTimer(pubcomp->getPacketID());
            SNPublish *publish = (SNPublish *)this->publishPackets->value(pubcomp->getPacketID());
            QString topicName = QString(publish->getTopic()->encode());
            emit pubackReceived(this, topicName, publish->getTopic()->getQoS()->getValue(), *publish->getContent(), publish->getIsDup(), publish->getIsRetain(), SN_ACCEPTED_RETURN_CODE);
            this->publishPackets->remove(pubcomp->getPacketID());
        } break;
        case SN_PUBREC:
        {
            SNPubrec *pubrec = (SNPubrec *)message;
            SNPublish *publish = (SNPublish *)this->timers->removeTimer(pubrec->getPacketID());
            this->publishPackets->insert(publish->getPacketID(), publish);
            SNPubrel *pubrel = new SNPubrel(pubrec->getPacketID());
            this->timers->goMessageTimer(pubrel);
        } break;
        case SN_PUBREL:
        {
            SNPubrel *pubrel = (SNPubrel *)message;
            this->timers->removeTimer(pubrel->getPacketID());
            SNPublish *publish = (SNPublish *)this->publishPackets->value(pubrel->getPacketID());
            SNPubcomp *pubcomp = new SNPubcomp(pubrel->getPacketID());
            this->send(pubcomp);
            QString topicName = QString(publish->getTopic()->encode());
            emit pubackReceived(this, topicName, publish->getTopic()->getQoS()->getValue(), *publish->getContent(), publish->getIsDup(), publish->getIsRetain(), SN_ACCEPTED_RETURN_CODE);
        } break;
        case SN_SUBSCRIBE:
        {
            emit errorReceived(this, "Packet 'Subscribe' did receive");
        } break;
        case SN_SUBACK:
        {
            SNSuback *suback = (SNSuback *)message;
            SNSubscribe *subscribe = (SNSubscribe *)this->timers->removeTimer(suback->getPacketID());
            QString topicName = QString(subscribe->getTopic()->encode());
            emit subackReceived(this, topicName, subscribe->getTopic()->getQoS()->getValue(), suback->getCode());
        } break;
        case SN_UNSUBSCRIBE:
        {
            emit errorReceived(this, "Packet 'Unsubscribe' did receive");
        } break;
        case SN_UNSUBACK:
        {
            SNUnsuback *unsuback = (SNUnsuback *)message;
            SNUnsubscribe *unsubscribe = (SNUnsubscribe *)this->timers->removeTimer(unsuback->getPacketID());
            QString topicName = QString(unsubscribe->getTopic()->encode());
            emit unsubackReceived(this, topicName);
        } break;
        case SN_PINGREQ:
        {
            emit errorReceived(this, "Packet 'Pingreq' did receive");
        } break;
        case SN_PINGRESP:
        {
            emit pingrespReceived(this);
        } break;
        case SN_DISCONNECT:
        {
            this->timers->stopAllTimers();
            emit disconnectReceived(this);
        } break;
        case SN_WILLTOPICUPD:
        {
            emit errorReceived(this, "Packet 'Will topic upd' did receive");
        } break;
        case SN_WILLTOPICRESP:
        {
            emit errorReceived(this, "Packet 'Will msg upd' did receive");
        } break;
        case SN_WILLMSGUPD:
        {
            emit errorReceived(this, "Packet 'Will topic resp' did receive");
        } break;
        case SN_WILLMSGRESP:
        {
            emit errorReceived(this, "Packet 'Will msg resp' did receive");
        } break;
        case SN_ENCAPSULATED:
        {
            emit errorReceived(this, "Packet 'Encapsulated' did receive");
        } break;
    }
}

void MqttSN::didFailWithError(InternetProtocol *protocol, QAbstractSocket::SocketError error)
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

void MqttSN::parseFailWithError(QString *error)
{
    emit errorReceived(this, *error);
}
