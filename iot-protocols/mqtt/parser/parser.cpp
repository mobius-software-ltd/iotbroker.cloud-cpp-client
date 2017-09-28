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

#include "parser.h"
#include "iot-protocols/mqtt/classes/mqttenums.h"

QByteArray Parser::next(QByteArray &byteArray)
{
    ByteArray *buffer = new ByteArray(byteArray);

    char fixedHeader = buffer->readChar();
    char messageType = ((fixedHeader >> 4) & 0xf);

    if (messageType == 0) {
        return QByteArray();
    }

    switch (messageType) {
        case MQ_PINGREQ:
        case MQ_PINGRESP:
        case MQ_DISCONNECT: {
            QByteArray dreturnArray = QByteArray(byteArray);
            dreturnArray.remove(2, byteArray.size());
            byteArray.remove(0, 2);
            return dreturnArray;
        }
        break;
        default:
            LengthDetails *length = LengthDetails::decode(buffer);

            if (length->getLength() == 0) {
                return QByteArray();
            }
            int result = length->getLength() + length->getSize() + 1;
            if (result > byteArray.size()) {
                qCritical(" -! invalid length decoding for %i result length: %i, in buffer: %i", messageType, result, byteArray.size());
            }
            QByteArray returnArray = QByteArray(byteArray);
            returnArray.remove(result, byteArray.size());
            byteArray.remove(0, result);

            return returnArray;
        break;
    }
    return NULL;
}

QByteArray Parser::encode(Message *message)
{
    int length = message->getLength();
    ByteArray *buffer = new ByteArray();
    unsigned char messageType = message->getType();

    switch (messageType) {

        case MQ_CONNECT: {

            Connect *connect = (Connect *)message;

            if (connect->isWillFlag() && !connect->getWill()->isValid()) {
                throw new QString("Encode. Connect. Invalid will encoding");
            }

            buffer->writeChar(messageType << 4);
            buffer->writeRawData(getBufferByLength(length));

            buffer->writeString(connect->getProtocol()->getName());
            buffer->writeChar(connect->getProtocolLevel());

            char contentFlags = 0;

            if (connect->isCleanSession() == true) {
                contentFlags += 2;
            }
            if (connect->isWillFlag() == true) {
                contentFlags += 4;
                int qosValue = connect->getWill()->getTopic()->getQoS()->getValue();
                contentFlags += qosValue << 3;
                if (connect->getWill()->isRetain() == true) {
                    contentFlags += 0x20;
                }
            }
            if (connect->isPasswordFlag() == true) {
                contentFlags += 0x40;
            }
            if (connect->isUsernameFlag() == true) {
                contentFlags += 0x80;
            }

            buffer->writeChar(contentFlags);
            buffer->writeShort(connect->getKeepAlive());
            buffer->writeString(connect->getClientID());

            if (connect->isWillFlag() == true) {
                QString willTopic = connect->getWill()->getTopic()->getName();
                if (willTopic != NULL) {
                    buffer->writeString(willTopic);
                }

                QByteArray willMessage = connect->getWill()->getContent();
                if (!willMessage.isEmpty()) {
                    buffer->writeShort(willMessage.length());
                    buffer->writeRawData(willMessage);
                }
            }

            if (connect->isUsernameFlag() == true) {
                buffer->writeString(connect->getUsername());
            }
            if (connect->isPasswordFlag() == true) {
                buffer->writeString(connect->getPassword());
            }
        }
        break;

        case MQ_CONNACK: {

            Connack *connack = (Connack *)message;
            buffer->writeChar(messageType << 4);
            buffer->writeRawData(getBufferByLength(length));
            buffer->writeChar(connack->isSessionPresent());
            buffer->writeChar(connack->getReturnCode());
        }
        break;

        case MQ_PUBLISH: {

            Publish *publish = (Publish *)message;
            char firstByte = (messageType << 4);
            firstByte |= (publish->isDup() == true)? 8 : 0;
            firstByte |= (publish->getTopic()->getQoS()->getValue() << 1);
            firstByte |= (publish->isRetain() == true)? 1 : 0;

            buffer->writeChar(firstByte);
            buffer->writeRawData(getBufferByLength(length));

            buffer->writeString(publish->getTopic()->getName());

            switch (publish->getTopic()->getQoS()->getValue()) {
                case AT_MOST_ONCE:
                    if (publish->getPacketID() != 0) {                        
                        throw new QString("Encode. Publish. Publish qos-0 must not contain packetID");
                    }
                break;
                case AT_LEAST_ONCE:
                case EXACTLY_ONCE:
                    if (publish->getPacketID() == 0) {
                        throw new QString("Encode. Publish. Publish qos-1,2 must contain packetID");
                    }
                    buffer->writeShort(publish->getPacketID());
                break;
            }
            buffer->writeRawData(publish->getContent());
        }
        break;

        case MQ_PUBACK: {

            Puback *puback = (Puback *)message;
            buffer->writeChar(messageType << 4);
            buffer->writeRawData(getBufferByLength(length));
            if (puback->getPacketID() == 0) {
                throw new QString("Encode. Puback. Puback must contain packetID");
            }
            buffer->writeShort(puback->getPacketID());
        }
        break;

        case MQ_PUBREC: {

            Pubrec *pubrec = (Pubrec *)message;
            buffer->writeChar(messageType << 4);
            buffer->writeRawData(getBufferByLength(length));
            if (pubrec->getPacketID() == 0) {
                throw new QString("Encode. Pubrec. Pubrec must contain packetID");
            }
            buffer->writeShort(pubrec->getPacketID());
        }
        break;

        case MQ_PUBREL: {

            Pubrel *pubrel = (Pubrel *)message;
            buffer->writeChar(messageType << 4 | 0x2);
            buffer->writeRawData(getBufferByLength(length));
            if (pubrel->getPacketID() == 0) {
                throw new QString("Encode. Pubrel. Pubrel must contain packetID");
            }
            buffer->writeShort(pubrel->getPacketID());
        }
        break;

        case MQ_PUBCOMP: {

            Pubcomp *pubcomp = (Pubcomp *)message;
            buffer->writeChar(messageType << 4);
            buffer->writeRawData(getBufferByLength(length));
            if (pubcomp->getPacketID() == 0) {
                throw new QString("Encode. Pubcomp. Pubcomp must contain packetID");
            }
            buffer->writeShort(pubcomp->getPacketID());
        }
        break;

        case MQ_SUBSCRIBE: {

            Subscribe *subscribe = (Subscribe *)message;
            buffer->writeChar(messageType << 4 | 0x2);
            buffer->writeRawData(getBufferByLength(length));
            if (subscribe->getPacketID() == 0) {
                throw new QString("Encode. Subscribe. Subscribe must contain packetID");
            }
            buffer->writeShort(subscribe->getPacketID());

            for (int i = 0; i < subscribe->getTopics()->size(); i++) {
                MQTopic item = subscribe->getTopics()->at(i);
                buffer->writeString(item.getName());
                buffer->writeChar(item.getQoS()->getValue());
            }
        }
        break;

        case MQ_SUBACK: {

            Suback *suback = (Suback *)message;
            buffer->writeChar(messageType << 4);
            buffer->writeRawData(getBufferByLength(length));
            if (suback->getPacketID() == 0) {
                throw new QString("Encode. Suback. Suback must contain packetID");
            }
            buffer->writeShort(suback->getPacketID());

            for (int i = 0; i < suback->getReturnCodes()->size(); i++) {
                SubackCode item = suback->getReturnCodes()->at(i);
                buffer->writeChar(item);
            }
        }
        break;

        case MQ_UNSUBSCRIBE: {

            Unsubscribe *unsubscribe = (Unsubscribe *)message;
            buffer->writeChar(messageType << 4 | 0x2);
            buffer->writeRawData(getBufferByLength(length));
            if (unsubscribe->getPacketID() == 0) {
                throw new QString("Encode. Unsubscribe. Unsubscribe must contain packetID");
            }
            buffer->writeShort(unsubscribe->getPacketID());

            for (int i = 0; i < unsubscribe->getTopics()->size(); i++) {
                QString item = unsubscribe->getTopics()->at(i);
                buffer->writeString(item);
            }
        }
        break;

        case MQ_UNSUBACK: {

            Unsuback *unsuback = (Unsuback *)message;
            buffer->writeChar(messageType << 4);
            buffer->writeRawData(getBufferByLength(length));
            if (unsuback->getPacketID() == 0) {
                throw new QString("Encode. Unsuback. Unsuback must contain packetID");
            }
            buffer->writeShort(unsuback->getPacketID());
        }
        break;

        case MQ_DISCONNECT:
        case MQ_PINGREQ:
        case MQ_PINGRESP:
            buffer->writeChar(messageType << 4);
            buffer->writeRawData(getBufferByLength(length));
        break;

        default: throw new QString("Encode. Type. Wrong message type"); break;
    }
    return buffer->getByteArray();
}

Message *Parser::decode(QByteArray byteArray)
{
    Message *message = NULL;

    ByteArray *buffer = new ByteArray(byteArray);

    char fixedHeader = buffer->readChar();
    char messageType = ((fixedHeader >> 4) & 0xf);

    LengthDetails *length = LengthDetails::decode(buffer);

    switch (messageType) {

        case MQ_CONNECT: {

            Connect *connect = new Connect();

            QString protocolName = buffer->readString();
            int protocolLevel = buffer->readChar();

            if (protocolName != connect->getProtocol()->getName()) {
                throw new QString("Decode. Connect. Protocol name is wrong");
            }

            char contentFlags = buffer->readChar();

            bool usernameFlag = (((contentFlags >> 7) & 1) == 1) ? true : false;
            bool passwordFlag = (((contentFlags >> 6) & 1) == 1) ? true : false;
            bool willRetainFlag = (((contentFlags >> 5) & 1) == 1) ? true : false;

            int willQoSFlag = (((contentFlags & 0x1f) >> 3) & 3);
            QoS *willQoS = new QoS(willQoSFlag);

            if (willQoS->isValidForMQTT() != true) {
                throw new QString(QString("Decode. Connect. Will QoS set to ") + QString::number(willQoS->getValue()));
            }

            bool willFlag = (((contentFlags >> 2) & 1) == 1) ? true : false;

            if (willQoS->getValue() > 0 && !willFlag) {
                throw new QString("Decode. Connect. Will retain set, willFlag not set");
            }

            bool cleanSessionFlag = (((contentFlags >> 1) & 1) == 1) ? true : false;
            bool reservedFlag = ((contentFlags  & 1) == 1) ? true : false;

            if (reservedFlag == true) {
                throw new QString("Decode. Connect. Reserved flag set to true");
            }

            int keepalive = buffer->readShort();

            QString clientID = buffer->readString();

            QByteArray willMessage;
            Will *will = NULL;

            if (willFlag == true) {

                QString willTopicName = buffer->readString();
                QString willMessageString = buffer->readString();

                willMessage.append(willMessageString);

                if (willTopicName.length() == 0) {
                    throw new QString("Decode. Connect. Will topic contains invalid will encoding");
                }

                MQTopic *topic = new MQTopic(willTopicName, willQoS);
                will = new Will(topic, willMessage, willRetainFlag);

                if (will->isValid() == false) {
                    throw new QString("Decode. Connect. Will contains invalid will encoding");
                }
            }

            QString username;

            if (usernameFlag == true) {
                username = buffer->readString();
            }

            QString password;

            if (passwordFlag == true) {
                password = buffer->readString();
            }

            connect->setUsername(username);
            connect->setPassword(password);
            connect->setClientID(clientID);
            connect->setCleanSession(cleanSessionFlag);
            connect->setKeepAlive(keepalive);
            connect->setWill(will);

            if (protocolLevel != 4) {
                connect->setProtocolLevel(protocolLevel);
            }
            message = connect;
        }
        break;

        case MQ_CONNACK: {

            Connack *connack = new Connack();

            char sessionPresentValue = buffer->readChar();

            if (sessionPresentValue != 0 && sessionPresentValue != 1) {
                throw new QString(QString("Decode. Connack. Session-present set to ")+QString::number(sessionPresentValue));
            }

            bool isPresent = (sessionPresentValue == 1) ? true : false;
            MQConnackCode connectReturnCode = (MQConnackCode)buffer->readChar();

            if (connack->isValidReturnCode(connectReturnCode) != true) {
                throw new QString("Decode. Connack. Invalid connack code");
            }

            connack->setSessionPresent(isPresent);
            connack->setReturnCode(connectReturnCode);

            message = connack;
        }
        break;

        case MQ_PUBLISH: {

            int dataLength = length->getLength();
            fixedHeader &= 0xf;

            bool dup = (((fixedHeader >> 3) & 1) == 1)? true : false;

            QoS *qos = new QoS((fixedHeader & 0x07) >> 1);

            if (qos->isValidForMQTT() == false) {
                throw new QString("Decode. Publish. Invalid QoS value");
            }

            if (dup == true && qos->getValue() == AT_MOST_ONCE) {
                throw new QString("Decode. Publish. QoS-0 dup flag present");
            }

            bool isRetain = ((fixedHeader & 1) == 1) ? true : false;

            QString topicName = buffer->readString();

            dataLength -= topicName.length() + 2;

            int packetID = 0;

            if (qos->getValue() != AT_MOST_ONCE) {
                packetID = buffer->readShort();
                if (packetID < 0 || packetID > 65535) {
                    throw new QString("Decode. Publish. Invalid Publish packetID encoding");
                }
                dataLength -= 2;
            }

            QByteArray data;

            if (dataLength > 0) {
                QString string = buffer->readStringWithLength(dataLength);
                data = string.toUtf8();
            }

            MQTopic *topic = new MQTopic(topicName, qos);
            message = new Publish(packetID, topic, data, isRetain, dup);
        }
        break;

        case MQ_PUBACK: {

            int packetID = buffer->readShort();
            message = new Puback(packetID);
        }
        break;

        case MQ_PUBREC: {

            int packetID = buffer->readShort();
            message = new Pubrec(packetID);
        }
        break;

        case MQ_PUBREL: {

            int packetID = buffer->readShort();
            message = new Pubrel(packetID);
        }
        break;

        case MQ_PUBCOMP: {

            int packetID = buffer->readShort();
            message = new Pubcomp(packetID);
        }
        break;

        case MQ_SUBSCRIBE: {

            int subscibeID = buffer->readShort();
            QList<MQTopic> *subscriptions = new QList<MQTopic>();

            while (buffer->getSize() > 0) {

                QString topic = buffer->readString();
                QoS *qos = new QoS(buffer->readChar());
                if (qos->isValidForMQTT() == false) {
                    throw new QString("Decode. Subscribe. Subscribe qos must be in range from 0 to 2");
                }

                MQTopic *subscription = new MQTopic(topic, qos);
                subscriptions->append(*subscription);
            }

            Subscribe *subscibe = new Subscribe(subscibeID);
            subscibe->setTopics(subscriptions);

            message = subscibe;
        }
        break;

        case MQ_SUBACK: {

            int subackID = buffer->readShort();
            QList<SubackCode> *subackCodes = new QList<SubackCode>();

            Suback *suback = new Suback(subackID);

            while (buffer->getSize() > 0) {

                SubackCode subackByte = (SubackCode)buffer->readChar();
                if (suback->isValidCode(subackByte) != true) {
                    throw new QString("Decode. Suback. Invalid suback code");
                }
                subackCodes->append(subackByte);
            }
            suback->setReturnCodes(subackCodes);
            message = suback;
        }
        break;

        case MQ_UNSUBSCRIBE: {

            int unsubackID = buffer->readShort();
            QList<QString> *unsubscribeTopics = new QList<QString>();

            while (buffer->getSize() > 0) {
                QString topic = buffer->readString();
                unsubscribeTopics->append(topic);
            }

            Unsubscribe *unsuback = new Unsubscribe(unsubackID, unsubscribeTopics);
            message = unsuback;
        }
        break;

        case MQ_UNSUBACK: {

            int packetID = buffer->readShort();
            message = new Unsuback(packetID);
        }
        break;

        case MQ_DISCONNECT: {

            message = new Disconnect();
        }
        break;

        case MQ_PINGREQ: {

            message = new Pingreq();
        }
        break;

        case MQ_PINGRESP:{

            message = new Pingresp();
        }
        break;


        default:  throw new QString("Decode. Type. Wrong message type"); break;
    }

    if (length->getLength() != message->getLength()) {
        throw new QString(QString("Decode. Length. Invalid length. Encoded: ")+QString::number(length->getLength())+QString(", actual: ")+QString::number(message->getLength()));
    }

    return message;
}

QByteArray Parser::getBufferByLength(int length)
{
    QByteArray byteArray = QByteArray();
    int lng = length;
    unsigned char encodedByte;

    do {
        encodedByte = (unsigned char)(lng % 128);
        lng /= 128;

        if (lng > 0) {
            byteArray.append((unsigned char)(encodedByte | 128));
        } else {
            byteArray.append(encodedByte);
        }
    } while (lng > 0);

    return byteArray;
}
