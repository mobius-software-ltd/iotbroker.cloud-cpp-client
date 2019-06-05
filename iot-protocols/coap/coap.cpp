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

#include "coap.h"
#include "internet-protocols/udpsocket.h"
#include "internet-protocols/dtlssocket.h"
#include "classes/convertor.h"
#include "iot-protocols/coap/classes/coapoptionparser.h"

CoAP::CoAP(AccountEntity account)
{
    this->clientId = account.clientID;
    this->currentAccount = account;
    this->timers = new TimersMap(this);
    this->messageParser = new CoAPParser();
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

bool CoAP::send(Message *message)
{
    if (this->internetProtocol->getState() == IP_CONNECTION_OPEN) {
        QByteArray data = this->messageParser->encode(message);
        return this->internetProtocol->send(data);
    }
    return false;
}

void CoAP::goConnect()
{
    if (this->currentAccount.isSecure) {
        if (!this->internetProtocol->setCertificate(this->currentAccount.keyPath, this->currentAccount.keyPass)) {
            return;
        }
    }
    this->internetProtocol->start();
}

void CoAP::publish(MessageEntity message)
{
    unsigned char qos = message.qos >= 1 ? 1 : message.qos;

    QString topicName = message.topicName.get().toString();
    QString content = QString(message.content.get().toByteArray());
    CoAPMessage *coapMessage = new CoAPMessage(COAP_PUT_METHOD, true, content);
    coapMessage->addOption(COAP_URI_PATH_OPTION, topicName);
    coapMessage->addOption(COAP_NODE_ID_OPTION, this->clientId);
    coapMessage->addOption(CoapOptionParser::encode(COAP_ACCEPT_OPTION, QVariant(qos)));
    this->timers->goCoAPMessageTimer(coapMessage, (qos == AT_MOST_ONCE));
}

void CoAP::subscribeTo(TopicEntity topic)
{
    unsigned char qos = topic.qos >= 1 ? 1 : topic.qos;

    QString topicName = topic.topicName.get().toString();
    CoAPMessage *coapMessage = new CoAPMessage(COAP_GET_METHOD, true, QString());
    coapMessage->addOption(COAP_URI_PATH_OPTION, topicName);
    coapMessage->addOption(COAP_NODE_ID_OPTION, this->clientId);
    coapMessage->addOption(CoapOptionParser::encode(COAP_OBSERVE_OPTION, QVariant(0)));
    coapMessage->addOption(CoapOptionParser::encode(COAP_ACCEPT_OPTION, QVariant(qos)));
    int token = this->timers->goCoAPMessageTimer(coapMessage, false);
    this->topics->insert(token, topicName);
}

void CoAP::unsubscribeFrom(TopicEntity topic)
{
    unsigned char qos = topic.qos >= 1 ? 1 : topic.qos;

    QString topicName = topic.topicName.get().toString();

    CoAPMessage *coapMessage = new CoAPMessage(COAP_GET_METHOD, true, QString());
    coapMessage->addOption(COAP_URI_PATH_OPTION, topicName);
    coapMessage->addOption(COAP_NODE_ID_OPTION, this->clientId);
    coapMessage->addOption(CoapOptionParser::encode(COAP_OBSERVE_OPTION, QVariant(1)));
    coapMessage->addOption(CoapOptionParser::encode(COAP_ACCEPT_OPTION, QVariant(qos)));
    int token = this->timers->goCoAPMessageTimer(coapMessage, false);
    this->topics->insert(token, topicName);
}

void CoAP::pingreq()
{

}

void CoAP::disconnectWith(int duration)
{
    Q_UNUSED(duration);
    this->timers->stopAllTimers();
    this->internetProtocol->stop();
}

Message *CoAP::getPingreqMessage()
{
    CoAPMessage *coapMessage = new CoAPMessage(COAP_PUT_METHOD, true, "");
    coapMessage->addOption(COAP_NODE_ID_OPTION, this->clientId);
    return coapMessage;
}

void CoAP::timeoutMethod()
{
    this->isConnect = false;
    this->timers->stopAllTimers();
    emit timeout(this);
}

// SLOTS

void CoAP::connectionDidStart(InternetProtocol *protocol)
{
    Q_UNUSED(protocol);
    this->isConnect = true;
    this->isConnackReceived = false;
    this->timers->goPingTimer(this->currentAccount.keepAlive);
}

void CoAP::connectionDidStop(InternetProtocol *protocol)
{
    Q_UNUSED(protocol);
    this->isConnect = false;
    this->timers->stopAllTimers();
}

void CoAP::didReceiveMessage(InternetProtocol *protocol, QByteArray data)
{
    if(!this->isConnackReceived) {
        this->isConnackReceived = true;
        emit connackReceived(this, 0);
    }
    Q_UNUSED(protocol);

    CoAPMessage *message = (CoAPMessage *)this->messageParser->decode(data);

    if ((message->getCode() == COAP_POST_METHOD || message->getCode() == COAP_PUT_METHOD) && message->getType() != COAP_ACKNOWLEDGMENT_TYPE) {
        QVariant qosVariant = CoapOptionParser::decode(message->getOption(COAP_ACCEPT_OPTION));
        QVariant topicNameVariant = CoapOptionParser::decode(message->getOption(COAP_URI_PATH_OPTION));
        int qos = qosVariant.toInt();
        QString topicName = topicNameVariant.toString();
        if (topicName.size() > 0) {
            QByteArray content = message->getPayload();
            emit publishReceived(this, topicName, qos, content, false, false);
        } else {
            CoAPMessage *ack = new CoAPMessage(COAP_METHOD_NOT_ALLOWED_RESPONSE_CODE, false, QString());
            ack->addOption(COAP_CONTENT_FORMAT_OPTION, QString("text/plain"));
            ack->setType(COAP_ACKNOWLEDGMENT_TYPE);
            ack->setMessageID(message->getMessageID());
            ack->setToken(message->getToken());
            this->send(ack);
            return;
        }
    }

    switch (message->getType()) {
    case COAP_CONFIRMABLE_TYPE: {
        CoAPMessage *ack = new CoAPMessage(COAP_PUT_METHOD, true, "");
        ack->addOption(COAP_NODE_ID_OPTION, this->clientId);
        ack->setType(COAP_ACKNOWLEDGMENT_TYPE);
        ack->setMessageID(message->getMessageID());
        ack->setToken(message->getToken());
        this->send(ack);
    } break;
    case COAP_NONCONFIRMABLE_TYPE: {
        this->timers->removeTimer(message->getToken());
    } break;
    case COAP_ACKNOWLEDGMENT_TYPE: {
        CoAPMessage *ack = (CoAPMessage *)this->timers->removeTimer(message->getToken());
        if (ack != NULL) {
            if (ack->getCode() == COAP_CONTENT_RESPONSE_CODE) {
                QString topicName = this->topics->value(ack->getToken());
                if (topicName.size() > 0) {
                    QByteArray content = ack->getPayload();
                    emit publishReceived(this, topicName, 0, content, false, false);
                }
            }
            if (ack->getCode() == COAP_GET_METHOD) {
                QVariant obsVariant = CoapOptionParser::decode(ack->getOption(COAP_OBSERVE_OPTION));
                int observeOptionValue = obsVariant.toInt();
                QString topicName = this->topics->value(ack->getToken());
                if (observeOptionValue == 0) {
                    QVariant qosVariant = CoapOptionParser::decode(ack->getOption(COAP_ACCEPT_OPTION));
                    int qos = qosVariant.toInt();
                    emit subackReceived(this, topicName, qos, 0);
                } else if (observeOptionValue == 1) {
                    emit unsubackReceived(this, topicName);
                }
            } else if (ack->getCode() == COAP_PUT_METHOD) {
                QVariant qosVariant = CoapOptionParser::decode(ack->getOption(COAP_ACCEPT_OPTION));
                QVariant topicNameVariant = CoapOptionParser::decode(ack->getOption(COAP_URI_PATH_OPTION));
                int qos = qosVariant.toInt();
                QString topicName = topicNameVariant.toString();
                if (qos != AT_MOST_ONCE) {
                    QByteArray content = ack->getPayload();
                    emit pubackReceived(this, topicName, qos, content, false, false, 0);
                }
            }
        }
    } break;
    case COAP_RESET_TYPE: {
        this->timers->removeTimer(message->getToken());
    } break;
    }
}

void CoAP::didFailWithError(InternetProtocol *protocol, QString error)
{
    Q_UNUSED(protocol);
    emit errorReceived(this, error);
}

void CoAP::parseFailWithError(QString *error)
{
    emit errorReceived(this, *error);
}
