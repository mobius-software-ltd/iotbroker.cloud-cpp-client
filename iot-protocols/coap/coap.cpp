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

#include "coap.h"
#include "internet-protocols/udpsocket.h"

CoAP::CoAP(AccountEntity account)
{
    this->currentAccount = account;
    this->timers = new TimersMap(this);
    this->messageParser = new CoAPParser();
    this->internetProtocol = new UDPSocket(account.serverHost, account.port);

    QObject::connect(this->internetProtocol, SIGNAL(connectionDidStart(InternetProtocol*)),                             this, SLOT(connectionDidStart(InternetProtocol*)));
    QObject::connect(this->internetProtocol, SIGNAL(connectionDidStop(InternetProtocol*)),                              this, SLOT(connectionDidStop(InternetProtocol*)));
    QObject::connect(this->internetProtocol, SIGNAL(didReceiveMessage(InternetProtocol*,QByteArray)),                   this, SLOT(didReceiveMessage(InternetProtocol*,QByteArray)));
    QObject::connect(this->internetProtocol, SIGNAL(didFailWithError(InternetProtocol*,QAbstractSocket::SocketError)),  this, SLOT(didFailWithError(InternetProtocol*,QAbstractSocket::SocketError)));
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

}

void CoAP::publish(MessageEntity message)
{
    QString topicName = message.topicName.get().toString();
    QString content = QString(message.content.get().toByteArray());
    CoAPMessage *coapMessage = new CoAPMessage(COAP_PUT_METHOD, true, true, content);
    coapMessage->addOperation(COAP_URI_PATH_OPTION, topicName);
    coapMessage->setType(COAP_CONFIRMABLE_TYPE);
    this->startSendMessage(coapMessage);
}

void CoAP::subscribeTo(TopicEntity topic)
{
    QString topicName = topic.topicName.get().toString();

    CoAPMessage *coapMessage = new CoAPMessage(COAP_GET_METHOD, true, true, QString());
    coapMessage->addOperation(COAP_OBSERVE_OPTION, QString::number(0));
    coapMessage->addOperation(COAP_URI_PATH_OPTION, topicName);
    coapMessage->setType(COAP_CONFIRMABLE_TYPE);
    this->startSendMessage(coapMessage);
}

void CoAP::unsubscribeFrom(TopicEntity topic)
{
    QString topicName = topic.topicName.get().toString();

    CoAPMessage *coapMessage = new CoAPMessage(COAP_GET_METHOD, true, true, QString());
    coapMessage->addOperation(COAP_OBSERVE_OPTION, QString::number(1));
    coapMessage->addOperation(COAP_URI_PATH_OPTION, topicName);
    coapMessage->setType(COAP_CONFIRMABLE_TYPE);
    this->startSendMessage(coapMessage);
}

void CoAP::pingreq()
{

}

void CoAP::disconnectWith(int duration)
{

}

Message *CoAP::getPingreqMessage()
{
    return NULL;
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

    if (coapMessage->getIsTokenExist() == true) {
        coapMessage->setToken(this->token % INT_MAX);
    }

    this->timers->goCoAPMessageTimer(coapMessage);
}

// SLOTS

void CoAP::connectionDidStart(InternetProtocol *protocol)
{
    this->isConnect = true;
    emit connackReceived(this, 0);
}

void CoAP::connectionDidStop(InternetProtocol *protocol)
{
    this->isConnect = false;
    this->timers->stopAllTimers();
}

void CoAP::didReceiveMessage(InternetProtocol *protocol, QByteArray data)
{
    CoAPMessage *message = (CoAPMessage *)this->messageParser->decode(data);

    if (message->getCode() == COAP_POST_METHOD || message->getCode() == COAP_PUT_METHOD) {
        QString topic = message->getOptionMap().value(COAP_URI_PATH_OPTION).last();
        QByteArray content = message->getPayload().toUtf8();

        emit publishReceived(this, topic, 0, content, false, false);

        CoAPMessage *ack = new CoAPMessage(COAP_METHOD_NOT_ALLOWED_RESPONSE_CODE, false, true, QString());
        ack->addOperation(COAP_CONTENT_FORMAT_OPTION, QString("text/plain"));
        ack->setType(COAP_ACKNOWLEDGMENT_TYPE);
        ack->setMessageID(message->getMessageID());
        ack->setToken(message->getToken());
        this->send(ack);
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
            QString topic = message->getOptionMap().value(COAP_URI_PATH_OPTION).last();
            QByteArray content = message->getPayload().toUtf8();

            emit publishReceived(this, topic, 0, content, false, false);
        }
        if (ack->getCode() == COAP_GET_METHOD) {
            int observeOptionValue = ack->getOptionMap().value(COAP_OBSERVE_OPTION).last().toInt();
            QString topic = message->getOptionMap().value(COAP_URI_PATH_OPTION).last();

            if (observeOptionValue == 0) {
                emit subackReceived(this, topic, 0, 0);
            } else if (observeOptionValue == 1) {
                emit unsubackReceived(this, topic);
            }
        } else if (ack->getCode() == COAP_PUT_METHOD) {
            QList<QString> values = ack->getOptionMap().value(COAP_URI_PATH_OPTION);
            QString topic = values.last();
            QByteArray content = message->getPayload().toUtf8();

            emit pubackReceived(this, topic, 0, content, false, false, 0);
        }
    } break;
    case COAP_RESET_TYPE: {
        this->timers->removeTimer(message->getToken());
    } break;
    }
}

void CoAP::didFailWithError(InternetProtocol *protocol, QAbstractSocket::SocketError error)
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

void CoAP::parseFailWithError(QString *error)
{
    emit errorReceived(this, *error);
}
