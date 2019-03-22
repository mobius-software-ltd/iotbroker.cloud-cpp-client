/**
 * Mobius Software LTD
 * Copyright 2015-2018, Mobius Software LTD
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
#include "internet-protocols/dtlssocket.h"
#include "iot-protocols/mqtt-sn/classes/topics/snidentifiertopic.h"

MqttSN::MqttSN(AccountEntity account) : IotProtocol(account)
{
    this->timers = new TimersMap(this);
    this->keepAlive = account.keepAlive;

    this->messageParser = new SNMessagesParser(this);
    this->publishPackets = new QMap<int, Message *>();
    this->forPublish = new QMap<int, SNPublish *>();
    this->topics = new QMap<int, QString>();

    if (account.isSecure) {
        this->internetProtocol = new DtlsSocket(account.serverHost.get().toString(), account.port.get().toInt());
    } else {
        this->internetProtocol = new UDPSocket(account.serverHost.get().toString(), account.port.get().toInt());
    }

    QObject::connect(this->internetProtocol, SIGNAL(connectionDidStart(InternetProtocol*)),             this, SLOT(connectionDidStart(InternetProtocol*)));
    QObject::connect(this->internetProtocol, SIGNAL(connectionDidStop(InternetProtocol*)),              this, SLOT(connectionDidStop(InternetProtocol*)));
    QObject::connect(this->internetProtocol, SIGNAL(didReceiveMessage(InternetProtocol*,QByteArray)),   this, SLOT(didReceiveMessage(InternetProtocol*,QByteArray)));
    QObject::connect(this->internetProtocol, SIGNAL(didFailWithError(InternetProtocol*,QString)),       this, SLOT(didFailWithError(InternetProtocol*,QString)));
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
    if (this->currentAccount.isSecure) {
        if (!this->internetProtocol->setCertificate(this->currentAccount.keyPath.get().toString(), this->currentAccount.keyPass.get().toString())) {
            return;
        }
    }

    SNConnect *connect = new SNConnect();
    connect->setWillPresent(this->currentAccount.willTopic.get().toString().length() != 0);
    connect->setCleanSession(this->currentAccount.cleanSession.get().toBool());
    connect->setDuration(this->currentAccount.keepAlive.get().toInt());
    connect->setClientID(this->currentAccount.clientID.get().toString());

    this->connect = connect;
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

    SNPublish *publish = new SNPublish(0, topic, content, isDup, isRetain);

    int packetID = this->timers->goRegisterTimer(registerPacket);

    this->forPublish->insert(packetID, publish);

    SNPublish *p = new SNPublish(publish->getPacketID(), publish->getTopic(), publish->getContent(), publish->getIsDup(), publish->getIsRetain());
    this->publishPackets->insert(packetID, p);
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
    this->internetProtocol->stop();
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
    Q_UNUSED(protocol);
    this->isConnect = true;
    this->timers->goConnectTimer(this->connect);
}

void MqttSN::connectionDidStop(InternetProtocol *protocol)
{
    Q_UNUSED(protocol);
    this->isConnect = false;
    this->timers->stopAllTimers();
}

void MqttSN::didReceiveMessage(InternetProtocol *protocol, QByteArray data)
{
    Q_UNUSED(protocol);
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
            this->topics->insert(reg->getTopicID(), reg->getTopicName());
            this->send(regack);
        } break;
        case SN_REGACK:
        {
            SNRegack *regack = (SNRegack *)message;
            this->timers->stopRegisterTimer();

            if (regack->getCode() == SN_ACCEPTED_RETURN_CODE) {
                SNPublish *publish = this->forPublish->value(regack->getPacketID());
                if (publish != NULL) {
                    this->topics->insert(regack->getTopicID(), QString(publish->getTopic()->encode()));
                    SNIdentifierTopic *topic = new SNIdentifierTopic(regack->getTopicID(), publish->getTopic()->getQoS());
                    publish->setPacketID(regack->getPacketID());
                    publish->setTopic(topic);
                    if (publish->getTopic()->getQoS()->getValue() == AT_MOST_ONCE) {
                        this->send(publish);
                    } else {
                        this->timers->goMessageTimer(publish);
                    }
                }
            }

        } break;
        case SN_PUBLISH:
        {
            SNPublish *publish = (SNPublish *)message;
            if (publish->getTopic()->getQoS()->getValue() == AT_LEAST_ONCE) {
                int topicId = ByteArray(publish->getTopic()->encode()).readShort();
                SNPuback *puback = new SNPuback(topicId, publish->getPacketID(), SN_ACCEPTED_RETURN_CODE);
                this->send(puback);
            } else if (publish->getTopic()->getQoS()->getValue() == EXACTLY_ONCE) {
                SNPubrec *pubrec = new SNPubrec(publish->getPacketID());
                this->publishPackets->insert(publish->getPacketID(), publish);
                this->timers->goMessageTimer(pubrec);
            }
            int topicId = ByteArray(publish->getTopic()->encode()).readShort();
            QString topicName = this->topics->value(topicId);
            emit publishReceived(this, topicName, publish->getTopic()->getQoS()->getValue(), *publish->getContent(), publish->getIsDup(), publish->getIsRetain());
        } break;
        case SN_PUBACK:
        {
            SNPuback *puback = (SNPuback *)message;
            this->timers->removeTimer(puback->getPacketID());
            SNPublish *publish = (SNPublish *)this->publishPackets->value(puback->getPacketID());
            QString topicName = QString(publish->getTopic()->encode());
            emit pubackReceived(this, topicName, publish->getTopic()->getQoS()->getValue(), *publish->getContent(), publish->getIsDup(), publish->getIsRetain(), puback->getCode());
            this->forPublish->remove(puback->getPacketID());
            this->publishPackets->remove(puback->getPacketID());
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
            this->timers->removeTimer(pubrec->getPacketID());
            SNPubrel *pubrel = new SNPubrel(pubrec->getPacketID());
            this->timers->goMessageTimer(pubrel);  
        } break;
        case SN_PUBREL:
        {
            SNPubrel *pubrel = (SNPubrel *)message;
            this->timers->removeTimer(pubrel->getPacketID());
            SNPublish *publish = (SNPublish *)this->publishPackets->value(pubrel->getPacketID());
            int topicId = ByteArray(publish->getTopic()->encode()).readShort();
            QString topicName =  this->topics->value(topicId);
            emit pubackReceived(this, topicName, publish->getTopic()->getQoS()->getValue(), *publish->getContent(), publish->getIsDup(), publish->getIsRetain(), SN_ACCEPTED_RETURN_CODE);
            SNPubcomp *pubcomp = new SNPubcomp(pubrel->getPacketID());
            this->send(pubcomp);
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
            this->topics->insert(suback->getTopicID(), topicName);
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

void MqttSN::didFailWithError(InternetProtocol *protocol, QString error)
{
    Q_UNUSED(protocol);
    emit errorReceived(this, error);
}

void MqttSN::parseFailWithError(QString *error)
{
    emit errorReceived(this, *error);
}
