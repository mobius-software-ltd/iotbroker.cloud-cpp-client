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

#include "amqp.h"
#include "iot-protocols/amqp/classes/amqpsimpletype.h"
#include "internet-protocols/tcpsocket.h"

AMQP::AMQP(AccountEntity account) : IotProtocol(account)
{
    this->timers = new TimersMap(this);
    this->isSASLConfirm = false;
    this->isPublishAllow = false;
    this->transferMap = new AMQPTransferMap();

    this->messageParser = new AMQPParser();

    this->internetProtocol = new TCPSocket(account.serverHost, account.port);

    QObject::connect(this->internetProtocol, SIGNAL(connectionDidStart(InternetProtocol*)),                             this, SLOT(connectionDidStart(InternetProtocol*)));
    QObject::connect(this->internetProtocol, SIGNAL(connectionDidStop(InternetProtocol*)),                              this, SLOT(connectionDidStop(InternetProtocol*)));
    QObject::connect(this->internetProtocol, SIGNAL(didReceiveMessage(InternetProtocol*,QByteArray)),                   this, SLOT(didReceiveMessage(InternetProtocol*,QByteArray)));
    QObject::connect(this->internetProtocol, SIGNAL(didFailWithError(InternetProtocol*,QAbstractSocket::SocketError)),  this, SLOT(didFailWithError(InternetProtocol*,QAbstractSocket::SocketError)));
}

bool AMQP::send(Message *message)
{
    if (this->internetProtocol->getState() == IP_CONNECTION_OPEN) {
        ByteArray data = this->messageParser->encode((AMQPHeader *)message);
        return this->internetProtocol->send(data.getByteArray());
    }
    return false;
}

void AMQP::goConnect()
{
    AMQPProtoHeader *header = new AMQPProtoHeader();
    header->setProtocolId(AMQPProtocolIdSASL);
    header->setVersionMajor(1);
    header->setVersionMinor(0);
    header->setVersionRevision(0);

    this->send(header);
}

void AMQP::publish(MessageEntity message)
{
    QByteArray content = message.content.get().toString().toUtf8();

    if (this->isPublishAllow == true) {
        AMQPTransfer *transfer = new AMQPTransfer();
        transfer->setChannel(this->channel);
        transfer->setHandle(AMQPSimpleType::UIntToVariant(0));
        transfer->setDeliveryId(AMQPSimpleType::UIntToVariant(0));
        transfer->setSettled(AMQPSimpleType::boolToVariant(false));
        transfer->setMore(AMQPSimpleType::boolToVariant(false));
        transfer->setMessageFormat(new AMQPMessageFormat(0));

        AMQPMessageHeader *messageHeader = new AMQPMessageHeader();
        messageHeader->setDurable(AMQPSimpleType::boolToVariant(true));
        messageHeader->setPriority(AMQPSimpleType::UByteToVariant(true));
        messageHeader->setMilliseconds(AMQPSimpleType::UIntToVariant(true));

        AMQPData *data = new AMQPData();
        data->setData(ByteArray(content));

        QList<AMQPSection *> sections = QList<AMQPSection *>();

        transfer->addSections(sections);

        this->send(this->transferMap->addTransfer(transfer));
    }
}

void AMQP::subscribeTo(TopicEntity topic)
{
    QString topicName = topic.topicName.get().toString();
    QoS *qos = new QoS(topic.qos.get().toInt());

    AMQPFlow *flow = new AMQPFlow();
    flow->setChannel(this->channel);
    flow->setHandle(AMQPSimpleType::UIntToVariant(1));
    flow->setIncomingWindow(AMQPSimpleType::UIntToVariant(65535));
    flow->setOutgoingWindow(AMQPSimpleType::UIntToVariant(65535));
    flow->setLinkCredit(AMQPSimpleType::UIntToVariant(65535));
    flow->setNextOutgoingId(AMQPSimpleType::UIntToVariant(0));
    flow->setNextIncomingId(AMQPSimpleType::UIntToVariant(0));
    flow->setDrain(AMQPSimpleType::boolToVariant(false));
    flow->setEcho(AMQPSimpleType::boolToVariant(false));

    this->send(flow);
    emit subackReceived(this, topicName, qos->getValue(), 0);
}

void AMQP::unsubscribeFrom(TopicEntity topic)
{
    QString topicName = topic.topicName.get().toString();
    emit unsubackReceived(this, topicName);
}

void AMQP::pingreq()
{
    this->send(new AMQPPing());
}

void AMQP::disconnectWith(int duration)
{
    AMQPDetach *detach = new AMQPDetach();
    detach->setChannel(this->channel);
    detach->setHandle(AMQPSimpleType::UIntToVariant(1));
    detach->setClosed(AMQPSimpleType::boolToVariant(true));
    this->send(detach);

    this->timers->stopAllTimers();
}

Message *AMQP::getPingreqMessage()
{
    return new AMQPPing();
}

void AMQP::timeoutMethod()
{
    this->timers->stopAllTimers();
    emit timeout(this);
}

// SLOTS

void AMQP::connectionDidStart(InternetProtocol *protocol)
{
    this->isConnect = true;
    this->goConnect();
}

void AMQP::connectionDidStop(InternetProtocol *protocol)
{
    this->isConnect = false;
    this->timers->stopAllTimers();
}

void AMQP::didReceiveMessage(InternetProtocol *protocol, QByteArray data)
{
    ByteArray *buffer = new ByteArray(data);

    if (buffer->getSize() == 0) return;

    do {
        int length = this->messageParser->next(*buffer);

        if (length <= 0) {
            qDebug() << "WRONG AMQP VERSION: AMQP version must be 1.0.0.";
        }

        ByteArray *packet = new ByteArray(buffer->subFrom(0, length).getByteArray());
        ByteArray *sub = new ByteArray(buffer->subFrom(length, buffer->getSize() - length));
        buffer = sub;

        AMQPHeader *message = this->messageParser->decode(*packet);

        switch (message->getType()) {
            case AMQP_PROTOCOL_HEADER_CODE:
            {
                AMQPProtoHeader *protoHeader = (AMQPProtoHeader *)message;
                if (this->isSASLConfirm == true && protoHeader->getProtocolId() == AMQPProtocolId) {
                    this->channel = protoHeader->getChannel();

                    AMQPOpen *open = new AMQPOpen();
                    QUuid uuid = QUuid().createUuid();
                    open->setContainerId(uuid.toString());

                    open->setChannel(protoHeader->getChannel());
                    open->setHostname(this->internetProtocol->getHost());

                    this->send(open);
                }
            } break;
            case AMQP_OPEN_HEADER_CODE:
            {
                AMQPOpen *open = (AMQPOpen *)message;
                int idleTimeoutInSeconds = AMQPSimpleType::variantToUInt(open->getIdleTimeout()) / 1000;
                this->timers->goPingTimer(idleTimeoutInSeconds);

                AMQPBegin *begin = new AMQPBegin();
                begin->setChannel(this->channel);
                begin->setNextOutgoingID(AMQPSimpleType::UIntToVariant(0));
                begin->setIncomingWindow(AMQPSimpleType::UIntToVariant(2147483647));
                begin->setOutgoingWindow(AMQPSimpleType::UIntToVariant(0));

                this->send(begin);

            } break;
            case AMQP_BEGIN_HEADER_CODE:
            {
                AMQPAttach *attach = (AMQPAttach *)message;

                QUuid uuid = QUuid().createUuid();

                attach->setChannel(this->channel);
                attach->setName(uuid.toString());
                attach->setHandle(AMQPSimpleType::UIntToVariant(0));
                attach->setRole(new AMQPRoleCode(AMQP_SENDER_ROLE_CODE));
                attach->setSendCodes(new AMQPSendCode(AMQP_MIXED_SEND_CODE));
                AMQPSource *source = new AMQPSource();
                source->setAddress(QString("my_queue"));
                source->setDurable(new AMQPTerminusDurability(AMQP_NONE_TERMINUS_DURABILITIES));
                source->setTimeout(AMQPSimpleType::UIntToVariant(0));
                source->setDynamic(AMQPSimpleType::boolToVariant(false));
                attach->setSource(source);
                AMQPTarget *target = new AMQPTarget();
                target->setAddress(QString("my_queue"));
                target->setDurable(new AMQPTerminusDurability(AMQP_NONE_TERMINUS_DURABILITIES));
                target->setTimeout(AMQPSimpleType::UIntToVariant(0));
                target->setDynamic(AMQPSimpleType::boolToVariant(false));
                attach->setTarget(target);
                attach->setInitialDeliveryCount(AMQPSimpleType::UIntToVariant(1));

                this->send(attach);

            } break;
            case AMQP_ATTACH_HEADER_CODE:
            {
                AMQPAttach *attach = (AMQPAttach *)message;

                if (attach->getRole()->getValue() == AMQP_SENDER_ROLE_CODE) {
                    emit connackReceived(this, 0);
                } else {
                    AMQPAttach *attach = new AMQPAttach();

                    QUuid uuid = QUuid().createUuid();

                    attach->setChannel(this->channel);
                    attach->setName(uuid.toString());
                    attach->setHandle(AMQPSimpleType::UIntToVariant(1));
                    attach->setRole(new AMQPRoleCode(AMQP_RECEIVER_ROLE_CODE));
                    attach->setSendCodes(new AMQPSendCode(AMQP_MIXED_SEND_CODE));
                    AMQPSource *source = new AMQPSource();
                    source->setAddress(QString("my_queue"));
                    source->setDurable(new AMQPTerminusDurability(AMQP_NONE_TERMINUS_DURABILITIES));
                    source->setTimeout(AMQPSimpleType::UIntToVariant(0));
                    source->setDynamic(AMQPSimpleType::boolToVariant(false));
                    attach->setSource(source);
                    AMQPTarget *target = new AMQPTarget();
                    target->setAddress(QString("my_queue"));
                    target->setDurable(new AMQPTerminusDurability(AMQP_NONE_TERMINUS_DURABILITIES));
                    target->setTimeout(AMQPSimpleType::UIntToVariant(0));
                    target->setDynamic(AMQPSimpleType::boolToVariant(false));
                    attach->setTarget(target);
                    attach->setInitialDeliveryCount(AMQPSimpleType::UIntToVariant(1));

                    this->send(attach);
                }

            } break;
            case AMQP_FLOW_HEADER_CODE:
            {
                AMQPFlow *flow = (AMQPFlow *)message;
                if (this->channel == flow->getChannel()) {
                    this->isPublishAllow = true;
                }
            } break;
            case AMQP_TRANSFER_HEADER_CODE:
            {
                AMQPTransfer *transfer = (AMQPTransfer *)message;
                AMQPData *data = (AMQPData *)transfer->getData();

                if (AMQPSimpleType::variantToBool(transfer->getSettled()) == true) {
                    emit publishReceived(this, QString(), 0, data->getData().getByteArray(), false, false);
                } else {
                    AMQPDisposition *disposition = new AMQPDisposition();
                    disposition->setChannel(this->channel);
                    disposition->setRole(new AMQPRoleCode(AMQP_RECEIVER_ROLE_CODE));
                    disposition->setFirst(transfer->getDeliveryId());
                    disposition->setLast(transfer->getDeliveryId());
                    disposition->setSettled(AMQPSimpleType::boolToVariant(true));
                    disposition->setState(new AMQPAccepted());
                    this->send(disposition);

                    emit publishReceived(this, QString(), 1, data->getData().getByteArray(), false, false);
                }
            } break;
            case AMQP_DISPOSITION_HEADER_CODE:
            {
                AMQPDisposition *disposition = (AMQPDisposition *)message;

                int first = AMQPSimpleType::variantToUInt(disposition->getFirst());
                int second = AMQPSimpleType::variantToUInt(disposition->getLast());

                for (int i = first; i <= second; i++) {
                    AMQPTransfer *transfer = this->transferMap->removeTransferByDeliveryId(i);
                    AMQPData *data = (AMQPData *)transfer->getData();
                    emit pubackReceived(this, QString(), 1, data->getData().getByteArray(), false, false, 0);
                }
            } break;
            case AMQP_DETACH_HEADER_CODE:
            {
                AMQPDetach *detach = (AMQPDetach *)message;
                AMQPEnd *end = new AMQPEnd();
                end->setChannel(detach->getChannel());
                this->send(end);
            } break;
            case AMQP_END_HEADER_CODE:
            {
                AMQPEnd *end = (AMQPEnd *)message;
                AMQPClose *close = new AMQPClose();
                close->setChannel(end->getChannel());
                this->send(close);
            } break;
            case AMQP_CLOSE_HEADER_CODE:
            {
                this->timers->stopAllTimers();
                this->isSASLConfirm = false;
            } break;
            case AMQP_MECHANISMS_HEADER_CODE:
            {
                AMQPSASLMechanisms *mechanisms = (AMQPSASLMechanisms *)message;
                AMQPSASLInit *saslInit = new AMQPSASLInit();
                saslInit->setTypeValue(mechanisms->getTypeValue());
                saslInit->setChannel(mechanisms->getChannel());
                saslInit->setHostName(this->internetProtocol->getHost());
                saslInit->setMechanism(AMQPSimpleType::variantToSymbol(mechanisms->getMechanisms().at(0)));
                this->send(saslInit);
            } break;
            case AMQP_INIT_HEADER_CODE:
            {
                qDebug() << " > Init did recive";
            } break;
            case AMQP_CHALLENGE_HEADER_CODE:
            {
                qDebug() << " > Challenge did recive";
            } break;
            case AMQP_RESPONSE_HEADER_CODE:
            {
                qDebug() << " > Response did recive";
            } break;
            case AMQP_OUTCOME_HEADER_CODE:
            {
                AMQPSASLOutcome *outcome = (AMQPSASLOutcome *)message;

                if (outcome->getOutcomeCode()->getValue() == AMQP_OK_SASL_CODE) {
                    this->isSASLConfirm = true;
                    AMQPProtoHeader *header = new AMQPProtoHeader();
                    header->setProtocolId(AMQPProtocolId);
                    header->setVersionMajor(1);
                    header->setVersionMinor(0);
                    header->setVersionRevision(0);
                    this->send(header);
                } else if (outcome->getOutcomeCode()->getValue() == AMQP_AUTH_SASL_CODE) {
                    qDebug() << "Connection authentication failed";
                    qDebug() << "Due to an unspecified problem with the supplied";
                } else if (outcome->getOutcomeCode()->getValue() == AMQP_SYS_SASL_CODE) {
                    qDebug() << "Connection authentication failed";
                    qDebug() << "Due to a system error";
                } else if (outcome->getOutcomeCode()->getValue() == AMQP_SYS_PERM_SASL_CODE) {
                    qDebug() << "Connection authentication failed";
                    qDebug() << "Due to a system error that is unlikely to be cor- rected without intervention";
                } else if (outcome->getOutcomeCode()->getValue() == AMQP_SYS_TEMP_SASL_CODE) {
                    qDebug() << "Connection authentication failed";
                    qDebug() << "Due to a transient system error";
                }

            } break;
            case AMQP_PING_HEADER_CODE:
            {
                qDebug() << " > Ping did recive";
            } break;
        }

    } while(buffer->getSize() > 0);
}

void AMQP::didFailWithError(InternetProtocol *protocol, QAbstractSocket::SocketError error)
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
