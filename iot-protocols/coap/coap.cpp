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

CoAP::CoAP(AccountEntity account)
{
    this->clientId = account.clientID;
    this->currentAccount = account;
    this->timers = new TimersMap(this);
    this->messageParser = new CoAPParser();

    if (account.isSecure) {
        this->internetProtocol = new DtlsSocket(account.serverHost, account.port);
    } else {
        this->internetProtocol = new UDPSocket(account.serverHost, account.port);
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
    QString topicName = message.topicName.get().toString();
    QString content = QString(message.content.get().toByteArray());
    CoAPMessage *coapMessage = new CoAPMessage(COAP_PUT_METHOD, true, content);
    coapMessage->addOption(COAP_NODE_ID_OPTION, this->clientId);
    coapMessage->addOption(COAP_URI_PATH_OPTION, topicName);
    coapMessage->setType(COAP_CONFIRMABLE_TYPE);
    this->startSendMessage(coapMessage);
}

void CoAP::subscribeTo(TopicEntity topic)
{
    QString topicName = topic.topicName.get().toString();

    CoAPMessage *coapMessage = new CoAPMessage(COAP_GET_METHOD, true, QString());
    coapMessage->addOption(COAP_OBSERVE_OPTION, QString::number(0));
    coapMessage->addOption(COAP_NODE_ID_OPTION, this->clientId);
    coapMessage->addOption(COAP_URI_PATH_OPTION, topicName);
    coapMessage->setType(COAP_CONFIRMABLE_TYPE);
    this->startSendMessage(coapMessage);
}

void CoAP::unsubscribeFrom(TopicEntity topic)
{
    QString topicName = topic.topicName.get().toString();

    CoAPMessage *coapMessage = new CoAPMessage(COAP_GET_METHOD, true, QString());
    coapMessage->addOption(COAP_OBSERVE_OPTION, QString::number(1));
    coapMessage->addOption(COAP_NODE_ID_OPTION, this->clientId);
    coapMessage->addOption(COAP_URI_PATH_OPTION, topicName);
    coapMessage->setType(COAP_CONFIRMABLE_TYPE);
    this->startSendMessage(coapMessage);
}

void CoAP::pingreq()
{

}

void CoAP::disconnectWith(int duration)
{
    Q_UNUSED(duration);
    this->timers->stopAllTimers();
}

Message *CoAP::getPingreqMessage()
{
    CoAPMessage *coapMessage = new CoAPMessage(COAP_PUT_METHOD, true, "");
    coapMessage->addOption(COAP_NODE_ID_OPTION, this->clientId);
    return coapMessage;
}

void CoAP::timeoutMethod()
{
    this->timers->stopAllTimers();
    emit timeout(this);
}

// Private

void CoAP::startSendMessage(Message *message)
{
    CoAPMessage *coapMessage = (CoAPMessage *)message;
    this->messageID++;
    this->token++;

    coapMessage->setMessageID(this->messageID % 65536);
    coapMessage->setToken(this->token % LONG_MAX);

    this->timers->goCoAPMessageTimer(coapMessage);
}

// SLOTS

void CoAP::connectionDidStart(InternetProtocol *protocol)
{
    Q_UNUSED(protocol);
    this->isConnect = true;
    emit connackReceived(this, 0);
}

void CoAP::connectionDidStop(InternetProtocol *protocol)
{
    Q_UNUSED(protocol);
    this->isConnect = false;
    this->timers->stopAllTimers();
}

void CoAP::didReceiveMessage(InternetProtocol *protocol, QByteArray data)
{
    Q_UNUSED(protocol);
    CoAPMessage *message = (CoAPMessage *)this->messageParser->decode(data);

    if (message->getCode() == COAP_POST_METHOD || message->getCode() == COAP_PUT_METHOD) {
        QString topic = message->getOptionValue(COAP_URI_PATH_OPTION);
        if (topic.size() > 0) {
            QByteArray content = message->getPayload();
            emit publishReceived(this, topic, 0, content, false, false);
        } else {
            CoAPMessage *ack = new CoAPMessage(COAP_METHOD_NOT_ALLOWED_RESPONSE_CODE, false, QString());
            ack->addOption(COAP_CONTENT_FORMAT_OPTION, QString("text/plain"));
            ack->setType(COAP_ACKNOWLEDGMENT_TYPE);
            ack->setMessageID(message->getMessageID());
            ack->setToken(message->getToken());
            this->send(ack);
        }
    }

    switch (message->getType()) {
    case COAP_CONFIRMABLE_TYPE: {
        message->setType(COAP_ACKNOWLEDGMENT_TYPE);
        this->send(message);
    } break;
    case COAP_NONCONFIRMABLE_TYPE: {
        this->timers->removeTimer(message->getToken());
    } break;
    case COAP_ACKNOWLEDGMENT_TYPE: {
        CoAPMessage *ack = (CoAPMessage *)this->timers->removeTimer(message->getToken());
        if (message->getCode() == COAP_CONTENT_RESPONSE_CODE) {
            QString topic = message->getOptionValue(COAP_URI_PATH_OPTION);
            if (topic.size() > 0) {
                QByteArray content = message->getPayload();
                emit publishReceived(this, topic, 0, content, false, false);
            }
        }
        if (ack->getCode() == COAP_GET_METHOD) {
            int observeOptionValue = ack->getOptionValue(COAP_OBSERVE_OPTION).toInt();
            QString topic = message->getOptionValue(COAP_URI_PATH_OPTION);
            if (observeOptionValue == 0) {
                emit subackReceived(this, topic, 0, 0);
            } else if (observeOptionValue == 1) {
                emit unsubackReceived(this, topic);
            }
        } else if (ack->getCode() == COAP_PUT_METHOD) {
            QString topic = ack->getOptionValue(COAP_URI_PATH_OPTION);
            QByteArray content = message->getPayload();

            emit pubackReceived(this, topic, 0, content, false, false, 0);
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
