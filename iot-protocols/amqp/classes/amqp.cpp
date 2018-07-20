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
#include "internet-protocols/sslsocket.h"

AMQP::AMQP(AccountEntity account) : IotProtocol(account)
{
    this->timers = new TimersMap(this);
    this->nextHandle = 0;
    this->isSASLConfirm = false;

    this->messageParser = new AMQPParser();

    this->usedIncomingMappings = QMap<QString, long>();
    this->usedOutgoingMappings = QMap<QString, long>();
    this->usedMappings = QMap<long, QString>();
    this->pendingMessages = QList<AMQPTransfer *>();

    this->internetProtocol = new TCPSocket(account.serverHost, account.port);

    if (account.isSecure) {
        this->internetProtocol = new SslSocket(account.serverHost, account.port);
        this->internetProtocol->setCertificate(account.keyPath, account.keyPass);
    } else {
        this->internetProtocol = new TCPSocket(account.serverHost, account.port);
    }

    QObject::connect(this->internetProtocol, SIGNAL(connectionDidStart(InternetProtocol*)),             this, SLOT(connectionDidStart(InternetProtocol*)));
    QObject::connect(this->internetProtocol, SIGNAL(connectionDidStop(InternetProtocol*)),              this, SLOT(connectionDidStop(InternetProtocol*)));
    QObject::connect(this->internetProtocol, SIGNAL(didReceiveMessage(InternetProtocol*,QByteArray)),   this, SLOT(didReceiveMessage(InternetProtocol*,QByteArray)));
    QObject::connect(this->internetProtocol, SIGNAL(didFailWithError(InternetProtocol*,QString)),       this, SLOT(didFailWithError(InternetProtocol*,QString)));
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
    if (this->currentAccount.isSecure) {
        if (!this->internetProtocol->setCertificate(this->currentAccount.keyPath, this->currentAccount.keyPass)) {
            return;
        }
    }

    this->internetProtocol->start();
}

void AMQP::publish(MessageEntity message)
{
    QString topicName = message.topicName.get().toString();
    QByteArray content = message.content.get().toString().toUtf8();

    AMQPTransfer *transfer = new AMQPTransfer();
    transfer->setChannel(this->channel);
    transfer->setDeliveryId(AMQPSimpleType::UIntToVariant(0));
    transfer->setSettled(AMQPSimpleType::boolToVariant(false));
    transfer->setMore(AMQPSimpleType::boolToVariant(false));
    transfer->setMessageFormat(new AMQPMessageFormat(0));

    AMQPMessageHeader *messageHeader = new AMQPMessageHeader();
    messageHeader->setDurable(AMQPSimpleType::boolToVariant(true));
    messageHeader->setPriority(AMQPSimpleType::UByteToVariant(3));
    messageHeader->setMilliseconds(AMQPSimpleType::UIntToVariant(1000));

    AMQPData *data = new AMQPData();
    data->setData(ByteArray(content));

    QList<AMQPSection *> sections = QList<AMQPSection *>();
    sections.append(data);

    transfer->addSections(sections);

    if (this->usedOutgoingMappings.contains(topicName)) {
        long handle = this->usedOutgoingMappings.value(topicName);
        transfer->setHandle(AMQPSimpleType::UIntToVariant(handle));
        this->timers->goMessageTimer(transfer);
    } else {
        long currentHandler = this->nextHandle++;
        this->usedOutgoingMappings.insert(topicName, currentHandler);
        this->usedMappings.insert(currentHandler, topicName);
        transfer->setHandle(AMQPSimpleType::UIntToVariant(currentHandler));
        this->pendingMessages.append(transfer);

        AMQPAttach *attach = new AMQPAttach();
        attach->setChannel(this->channel);
        attach->setName(topicName);
        attach->setHandle(AMQPSimpleType::UIntToVariant(currentHandler));
        attach->setRole(new AMQPRoleCode(AMQP_SENDER_ROLE_CODE));
        attach->setSendCodes(new AMQPSendCode(AMQP_MIXED_SEND_CODE));

        AMQPSource *source = new AMQPSource();
        source->setAddress(topicName);
        source->setDurable(new AMQPTerminusDurability(AMQP_NONE_TERMINUS_DURABILITIES));
        source->setTimeout(AMQPSimpleType::UIntToVariant(0));
        source->setDynamic(AMQPSimpleType::boolToVariant(false));
        attach->setSource(source);

        this->send(attach);
    }
}

void AMQP::subscribeTo(TopicEntity topic)
{
    QString topicName = topic.topicName.get().toString();

    long currentHandler;
    if (this->usedIncomingMappings.contains(topicName)) {
        currentHandler = this->usedIncomingMappings.value(topicName);
    } else {
        currentHandler = nextHandle++;
        this->usedIncomingMappings.insert(topicName, currentHandler);
        this->usedMappings.insert(currentHandler, topicName);
    }

    AMQPAttach *attach = new AMQPAttach();
    attach->setChannel(this->channel);
    attach->setName(topicName);
    attach->setHandle(AMQPSimpleType::UIntToVariant(currentHandler));
    attach->setRole(new AMQPRoleCode(AMQP_RECEIVER_ROLE_CODE));
    attach->setSendCodes(new AMQPSendCode(AMQP_MIXED_SEND_CODE));
    AMQPTarget *target = new AMQPTarget();
    target->setAddress(topicName);
    target->setDurable(new AMQPTerminusDurability(AMQP_NONE_TERMINUS_DURABILITIES));
    target->setTimeout(AMQPSimpleType::UIntToVariant(0));
    target->setDynamic(AMQPSimpleType::boolToVariant(false));
    attach->setTarget(target);

    this->send(attach);
}

void AMQP::unsubscribeFrom(TopicEntity topic)
{
    QString topicName = topic.topicName.get().toString();

    if (this->usedIncomingMappings.contains(topicName)) {
        AMQPDetach *detach = new AMQPDetach();
        detach->setChannel(this->channel);
        detach->setClosed(AMQPSimpleType::boolToVariant(true));
        detach->setHandle(AMQPSimpleType::UIntToVariant(this->usedIncomingMappings.value(topicName)));
        this->send(detach);
    } else {
        emit unsubackReceived(this, topicName);
    }
}

void AMQP::pingreq()
{
    this->send(new AMQPPing());
}

void AMQP::disconnectWith(int duration)
{
    Q_UNUSED(duration);
    AMQPEnd *end = new AMQPEnd();
    end->setChannel(this->channel);
    this->send(end);
    if (this->timers != NULL) {
        this->timers->stopAllTimers();
    }
    this->isConnect = false;
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
    Q_UNUSED(protocol);
    this->isConnect = true;

    AMQPProtoHeader *header = new AMQPProtoHeader();
    header->setProtocolId(AMQPProtocolIdSASL);
    header->setVersionMajor(1);
    header->setVersionMinor(0);
    header->setVersionRevision(0);

    this->send(header);
}

void AMQP::connectionDidStop(InternetProtocol *protocol)
{
    Q_UNUSED(protocol);
    this->isConnect = false;
    this->timers->stopAllTimers();
}

void AMQP::didReceiveMessage(InternetProtocol *protocol, QByteArray data)
{
    Q_UNUSED(protocol);
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
                } else {
                    this->timers->stopAllTimers();
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
                this->isConnect = true;
            } break;
            case AMQP_ATTACH_HEADER_CODE:
            {
                AMQPAttach *attach = (AMQPAttach *)message;

                if (attach->getRole()->getValue() == AMQP_SENDER_ROLE_CODE) {
                    // publish
                    for (int i = 0; i < pendingMessages.size(); i++) {
                        AMQPTransfer *message = this->pendingMessages.at(i);
                        if (message->getHandle() == attach->getHandle()) {
                            this->pendingMessages.removeAt(i);
                            i--;

                            this->timers->goMessageTimer(message);
                        }
                    }
                } else {
                    // subscribe
                }

            } break;
            case AMQP_FLOW_HEADER_CODE:
            {
                // not implemented for now
            } break;
            case AMQP_TRANSFER_HEADER_CODE:
            {
                AMQPTransfer *transfer = (AMQPTransfer *)message;
                AMQPData *data = (AMQPData *)transfer->getData();

                QoS *qos = new QoS(AT_LEAST_ONCE);

                if (AMQPSimpleType::variantToBool(transfer->getSettled())) {
                    qos->setValue(AT_MOST_ONCE);
                } else {
                    AMQPDisposition *disposition = new AMQPDisposition();
                    disposition->setChannel(this->channel);
                    disposition->setRole(new AMQPRoleCode(AMQP_RECEIVER_ROLE_CODE));
                    disposition->setFirst(transfer->getDeliveryId());
                    disposition->setLast(transfer->getDeliveryId());
                    disposition->setSettled(AMQPSimpleType::boolToVariant(true));
                    disposition->setState(new AMQPAccepted());
                    this->send(disposition);
                }

                QString topic = QString();
                long handle = AMQPSimpleType::variantToUInt(transfer->getHandle());
                if (transfer->getHandle()->isNull() || !this->usedMappings.contains(handle)) {
                    return;
                }

                topic = this->usedMappings.value(handle);

                emit publishReceived(this, QString(), qos->getValue(), data->getData().getByteArray(), false, false);

            } break;
            case AMQP_DISPOSITION_HEADER_CODE:
            {
                AMQPDisposition *disposition = (AMQPDisposition *)message;

                unsigned int first = AMQPSimpleType::variantToUInt(disposition->getFirst());
                unsigned int second = AMQPSimpleType::variantToUInt(disposition->getLast());

                for (unsigned int i = first; i < second; i++) {
                    this->timers->removeTimer(i);
                }
            } break;
            case AMQP_DETACH_HEADER_CODE:
            {
                AMQPDetach *detach = (AMQPDetach *)message;
                long handle = AMQPSimpleType::variantToUInt(detach->getHandle());
                if (this->usedMappings.contains(handle)) {
                    QString topicName = this->usedMappings.value(handle);
                    this->usedMappings.remove(handle);
                    if (this->usedOutgoingMappings.contains(topicName)) {
                        this->usedOutgoingMappings.remove(topicName);
                    }
                }
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
                this->isConnect = false;
            } break;
            case AMQP_MECHANISMS_HEADER_CODE:
            {
                AMQPSASLMechanisms *mechanisms = (AMQPSASLMechanisms *)message;

                AMQPSymbol *plainMechanism = NULL;

                for (int i = 0; i < mechanisms->getMechanisms().size(); i++) {
                    AMQPSymbol *mechanism = AMQPSimpleType::variantToSymbol(mechanisms->getMechanisms().at(i));
                    if (mechanism->getValue().toLower() == "plain") {
                        plainMechanism = mechanism;
                        break;
                    }
                }

                if (plainMechanism == NULL) {
                    this->timers->stopAllTimers();
                    return;
                }

                AMQPSASLInit *saslInit = new AMQPSASLInit();
                saslInit->setTypeValue(mechanisms->getTypeValue());
                saslInit->setChannel(mechanisms->getChannel());
                saslInit->setHostName(this->internetProtocol->getHost());
                saslInit->setMechanism(plainMechanism);

                QByteArray userBytes = this->currentAccount.username.get().toString().toUtf8();
                QByteArray passwordBytes = this->currentAccount.password.get().toString().toUtf8();
                QByteArray challenge = QByteArray();

                challenge.append(userBytes);
                challenge.append((char) 0x00);
                challenge.append(userBytes);
                challenge.append((char) 0x00);
                challenge.append(passwordBytes);

                saslInit->setInitialResponse(challenge);

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

void AMQP::didFailWithError(InternetProtocol *protocol, QString error)
{
    Q_UNUSED(protocol);
    emit errorReceived(this, error);
}
