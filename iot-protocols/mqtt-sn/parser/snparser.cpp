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

#include "snparser.h"
#include "iot-protocols/mqtt-sn/classes/snflags.h"
#include "iot-protocols/mqtt-sn/classes/sncontrols.h"
#include "iot-protocols/mqtt-sn/classes/snvaluesvalidator.h"
#include "iot-protocols/mqtt-sn/classes/topics/snfulltopic.h"
#include "iot-protocols/mqtt-sn/classes/topics/snidentifiertopic.h"
#include "iot-protocols/mqtt-sn/classes/topics/snshorttopic.h"

static unsigned char const threeOctetLengthSuffix = 0x01;


QByteArray SNParser::encode(Message *message)
{
    ByteArray *data = new ByteArray();
    int length = message->getLength();

    if (length <= 255) {
        data->writeChar(length);
    } else {
        data->writeChar(threeOctetLengthSuffix);
        data->writeShort(length);
    }

    SNMessageType type = (SNMessageType)message->getType();
    data->writeChar(type);

    switch(type) {

        case SN_ADVERTISE:
        {
            SNAdvertise *advertise = (SNAdvertise *)message;
            data->writeChar(advertise->getGwId());
            data->writeShort(advertise->getDuration());
        } break;

        case SN_SEARCHGW:
        {
            SNSearchGW *searchGw = (SNSearchGW *)message;
            data->writeChar(searchGw->getRadius());
        } break;

        case SN_GWINFO:
        {
            SNGWInfo *gwInfo = (SNGWInfo *)message;
            data->writeChar(gwInfo->getGwID());
            if (gwInfo->getGwAddress().length() != 0) {
                data->writeRawData(gwInfo->getGwAddress().toUtf8());
            }
        } break;

        case SN_CONNECT:
        {
            SNConnect *connect = (SNConnect *)message;
            char connectFlagsByte = SNFlags::encode(false, NULL, false, connect->getWillPresent(), connect->getCleanSession(), SN_UNKNOWN_TOPIC_TYPE);
            data->writeChar(connectFlagsByte);
            data->writeChar(connect->getProtocolID());
            data->writeShort(connect->getDuration());
            data->writeRawData(connect->getClientID().toUtf8());
        } break;

        case SN_CONNACK:
        case SN_WILLTOPICRESP:
        case SN_WILLMSGRESP:
        {
            SNResponseMessage *responseMessage = (SNResponseMessage *)message;
            data->writeChar(responseMessage->getReturnCode());
        } break;

        case SN_WILLTOPIC:
        {
            SNWillTopic *willTopic = (SNWillTopic *)message;
            if (willTopic->getTopic() != NULL) {
                char willTopicFlagsByte = SNFlags::encode(false, willTopic->getTopic()->getQoS(), willTopic->getIsRetain(), false, false, willTopic->getTopic()->getType());
                data->writeChar(willTopicFlagsByte);
                data->writeRawData(willTopic->getTopic()->encode());
            }
        } break;

        case SN_WILLMSG:
        {
            SNWillMsg *willMsg = (SNWillMsg *)message;
            data->writeRawData(willMsg->getContent());
        } break;

        case SN_REGISTER:
        {
            SNRegister *registerSN = (SNRegister *)message;
            data->writeShort(registerSN->getTopicID());
            data->writeShort(registerSN->getPacketID());
            data->writeRawData(registerSN->getTopicName().toUtf8());
        } break;

        case SN_REGACK:
        {
            SNRegack *regack = (SNRegack *)message;
            data->writeShort(regack->getTopicID());
            data->writeShort(regack->getPacketID());
            data->writeChar(regack->getCode());
        } break;

        case SN_PUBLISH:
        {
            SNPublish *publish = (SNPublish *)message;
            char publishFlagsByte = SNFlags::encode(publish->getIsDup(), publish->getTopic()->getQoS(), publish->getIsRetain(), false, false, publish->getTopic()->getType());            
            data->writeChar(publishFlagsByte);
            data->writeRawData(publish->getTopic()->encode());
            data->writeShort(publish->getPacketID());
            data->writeRawData(*publish->getContent());
        } break;

        case SN_PUBACK:
        {
            SNPuback *puback = (SNPuback *)message;
            data->writeShort(puback->getTopicID());
            data->writeShort(puback->getPacketID());
            data->writeChar(puback->getCode());
        } break;

        case SN_PUBREC:
        case SN_PUBREL:
        case SN_PUBCOMP:
        case SN_UNSUBACK:
        {
            CountableMessage *contableMessage = (CountableMessage *)message;
            data->writeShort(contableMessage->getPacketID());
        } break;

        case SN_SUBSCRIBE:
        {
            SNSubscribe *subscribe = (SNSubscribe *)message;
            char subscribeFlags = SNFlags::encode(subscribe->getIsDup(), subscribe->getTopic()->getQoS(), false, false, false, subscribe->getTopic()->getType());
            data->writeChar(subscribeFlags);
            data->writeShort(subscribe->getPacketID());
            data->writeRawData(subscribe->getTopic()->encode());
        } break;

        case SN_SUBACK:
        {
            SNSuback *suback = (SNSuback *)message;
            char subackByte = SNFlags::encode(false, suback->getQos(), false, false, false, SN_UNKNOWN_TOPIC_TYPE);
            data->writeChar(subackByte);
            data->writeShort(suback->getTopicID());
            data->writeShort(suback->getPacketID());
            data->writeChar(suback->getCode());
        } break;

        case SN_UNSUBSCRIBE:
        {
            SNUnsubscribe *unsubscribe = (SNUnsubscribe *)message;
            char unsubscribeFlags = SNFlags::encode(false, NULL, false, false, false, unsubscribe->getTopic()->getType());
            data->writeChar(unsubscribeFlags);
            data->writeShort(unsubscribe->getPacketID());
            data->writeRawData(unsubscribe->getTopic()->encode());
        } break;

        case SN_PINGREQ:
        {
            if (length > 2) {
                SNPingreq *pingreq = (SNPingreq *)message;
                data->writeRawData(pingreq->getClientID().toUtf8());
            }
        } break;

        case SN_WILLTOPICREQ:
        case SN_WILLMSGREQ:
        case SN_PINGRESP:
        {
        } break;

        case SN_DISCONNECT:
        {
            if (length > 2) {
                SNDisconnect *disconnect = (SNDisconnect *)message;
                data->writeShort(disconnect->getDuration());
            }
        } break;

        case SN_WILLTOPICUPD:
        {
            SNWillTopicUpd *willTopicUpd = (SNWillTopicUpd *)message;
            if (willTopicUpd->getTopic() != NULL) {
                char willTopicUpdByte = SNFlags::encode(false, willTopicUpd->getTopic()->getQoS(), willTopicUpd->getIsRetain(), false, false, SN_UNKNOWN_TOPIC_TYPE);
                data->writeChar(willTopicUpdByte);
                data->writeRawData(willTopicUpd->getTopic()->encode());
            }
        } break;

        case SN_WILLMSGUPD:
        {
            SNWillMsgUpd *willMsgUpd = (SNWillMsgUpd *)message;
            data->writeRawData(willMsgUpd->getContent());
        } break;

        case SN_ENCAPSULATED:
        {
            SNEncapsulated *encapsulated = (SNEncapsulated *)message;
            data->writeChar(SNControls::encode(encapsulated->getRadius()));
            data->writeRawData(encapsulated->getWirelessNodeID().toUtf8());
            data->writeRawData(SNParser::encode(encapsulated->getMessage()));
        } break;

    }

    if (type != SN_ENCAPSULATED && message->getLength() != data->getSize()) {
        throw new QString("invalid message encoding: expected length = " + QString::number(message->getLength()) + " actual = " + QString::number(data->getSize()));
    }

    return data->getByteArray();
}

Message *SNParser::decode(QByteArray data)
{
    ByteArray *buffer = new ByteArray(data);

    Message *message = NULL;

    int messageLength = SNParser::decodeContentLength(buffer);
    int bytesLeft = messageLength - (messageLength - buffer->getSize());

    short typeByte = buffer->readChar();

    SNMessageType type = (SNMessageType)typeByte;

    switch(type) {

        case SN_ADVERTISE:
        {
            int advertiseGwID = buffer->readChar();
            int advertiseDuration = buffer->readShort();
            message = new SNAdvertise(advertiseGwID, advertiseDuration);
        } break;

        case SN_SEARCHGW:
        {
            SNRadius radius = (SNRadius)buffer->readChar();
            message = new SNSearchGW(radius);
        } break;

        case SN_GWINFO:
        {
            int gwInfoGWID = buffer->readChar();
            bytesLeft--;

            QString gwInfoGwAddress;
            if (bytesLeft > 0) {
                gwInfoGwAddress = buffer->readStringWithLength(bytesLeft);
            }
            message = new SNGWInfo(gwInfoGWID, gwInfoGwAddress);
        } break;

        case SN_CONNECT:
        {
            SNFlags *flags = SNFlags::decode(buffer->readChar(), type);
            bytesLeft--;

            int protocolID = buffer->readChar();
            bytesLeft--;

            if (protocolID != MQTT_SN_PROTOCOL_ID) {
                throw new QString("Invalid protocolID = "+QString::number(protocolID));
            }

            int connectDuration = buffer->readShort();

            bytesLeft -= 2;

            if (!SNValuesValidator::canReadData(*buffer, bytesLeft)) {
                throw new QString("Type = "+QString::number(type)+". clientID can't be empty");
            }

            QString clientId = buffer->readStringWithLength(bytesLeft);
            message = new SNConnect(flags->getCleanSession(), connectDuration, clientId, flags->getWill());
        } break;

        case SN_CONNACK:
        {
            SNReturnCode code = (SNReturnCode)buffer->readChar();
            message = new SNConnack(code);
        } break;

        case SN_WILLTOPICREQ:
        {
            message = new SNWillTopicReq();
        } break;

        case SN_WILLTOPIC:
        {
            bool willTopicRetain = false;
            SNFullTopic *willTopic = NULL;
            if (bytesLeft > 0) {
                SNFlags *willTopicFlags = SNFlags::decode(buffer->readChar(), type);
                bytesLeft--;
                willTopicRetain = willTopicFlags->getIsRetain();
                if (!SNValuesValidator::canReadData(*buffer, bytesLeft)) {
                    throw new QString("invalid topic encoding. Type = "+QString::number(type));
                }
                QString willTopicString = buffer->readStringWithLength(bytesLeft);
                willTopic = new SNFullTopic(willTopicString, willTopicFlags->getQos());
            }
            message = new SNWillTopic(willTopic, willTopicRetain);
        } break;

        case SN_WILLMSGREQ:
        {
            message = new SNWillMsgReq();
        } break;

        case SN_WILLMSG:
        {
            if (!SNValuesValidator::canReadData(*buffer, bytesLeft)) {
                throw new QString("content must not be empty. Type = "+QString::number(type));
            }
            QString willMSGContentString = buffer->readStringWithLength(bytesLeft);
            QByteArray willMSGContent = willMSGContentString.toUtf8();

            message = new SNWillMsg(willMSGContent);
        } break;

        case SN_REGISTER:
        {
            int registerTopicId = buffer->readShort();
            if (!SNValuesValidator::validateRegistrationTopicID(registerTopicId)) {
                throw new QString(QString::number(registerTopicId)+" is invalid topicID value. Type = "+QString::number(type));
            }
            bytesLeft -= 2;
            int registerPacketId = buffer->readShort();
            if (!SNValuesValidator::validatePacketID(registerPacketId)) {
                throw new QString(QString::number(registerPacketId)+" is invalid packetID. Type = "+QString::number(type));
            }
            bytesLeft -= 2;
            if (!SNValuesValidator::canReadData(*buffer, bytesLeft)) {
                throw new QString("must contain a valid topic. Type = "+QString::number(type));
            }
            QString registerTopicName = buffer->readStringWithLength(bytesLeft);
            message = new SNRegister(registerTopicId, registerPacketId, registerTopicName);
        } break;

        case SN_REGACK:
        {
            int regackTopicId = buffer->readShort();
            if (!SNValuesValidator::validateRegistrationTopicID(regackTopicId)) {
                throw new QString(QString::number(regackTopicId)+" is invalid topicID value. Type = "+QString::number(type));
            }
            int regackPacketId = buffer->readShort();
            if (!SNValuesValidator::validatePacketID(regackPacketId)) {
                throw new QString(QString::number(regackPacketId)+" is invalid packetID. Type = "+QString::number(type));
            }
            SNReturnCode code = (SNReturnCode)buffer->readChar();
            message = new SNRegack(regackTopicId, regackPacketId, code);
        } break;

        case SN_PUBLISH:
        {
            SNFlags *publishFlags = SNFlags::decode(buffer->readChar(), type);
            bytesLeft--;
            int publishTopicID = buffer->readShort();
            bytesLeft -= 2;
            int publishPacketID = buffer->readShort();
            bytesLeft -= 2;

            if (publishFlags->getQos()->getValue() != AT_MOST_ONCE && publishPacketID == 0) {
                throw new QString("invalid PUBLISH QoS-0. Type = "+QString::number(type));
            }

            Topic *publishTopic = NULL;

            if (publishFlags->getTopicType() == SN_SHORT_TOPIC_TYPE) {
                publishTopic = new SNShortTopic(QString::number(publishTopicID), publishFlags->getQos());
            } else {
                if (!SNValuesValidator::validateTopicID(publishTopicID)) {
                    throw new QString(QString::number(publishTopicID)+" is invalid topicID value. Type = "+QString::number(type));
                }
                publishTopic = new SNIdentifierTopic(publishTopicID, publishFlags->getQos());
            }

            QByteArray *content = NULL;
            if (bytesLeft > 0) {
                QString publishContentString = buffer->readStringWithLength(bytesLeft);
                content = new QByteArray(publishContentString.toUtf8());
            }
            message = new SNPublish(publishPacketID, publishTopic, content, publishFlags->getIsDup(), publishFlags->getIsRetain());
        } break;

        case SN_PUBACK:
        {
            int pubackTopicID = buffer->readShort();
            if (!SNValuesValidator::validateTopicID(pubackTopicID)) {
                throw new QString(QString::number(pubackTopicID)+" is invalid topicID value. Type = "+QString::number(type));
            }
            int pubackPacketID = buffer->readShort();
            if (!SNValuesValidator::validatePacketID(pubackPacketID)) {
                throw new QString(QString::number(pubackPacketID)+" is invalid packetID value. Type = "+QString::number(type));
            }
            SNReturnCode returnCode = (SNReturnCode)buffer->readChar();
            message = new SNPuback(pubackTopicID, pubackPacketID, returnCode);
        } break;

        case SN_PUBCOMP:
        {
            int pubcompPacketID = buffer->readShort();
            if (!SNValuesValidator::validatePacketID(pubcompPacketID)) {
                throw new QString(QString::number(pubcompPacketID)+" is invalid packetID value. Type = "+QString::number(type));
            }
            message = new SNPubcomp(pubcompPacketID);;
        } break;

        case SN_PUBREC:
        {
            int pubrecPacketID = buffer->readShort();
            if (!SNValuesValidator::validatePacketID(pubrecPacketID)) {
                throw new QString(QString::number(pubrecPacketID)+" is invalid packetID value. Type = "+QString::number(type));
            }
            message = new SNPubrec(pubrecPacketID);
        } break;

        case SN_PUBREL:
        {
            int pubrelPacketID = buffer->readShort();
            if (!SNValuesValidator::validatePacketID(pubrelPacketID)) {
                throw new QString(QString::number(pubrelPacketID)+" is invalid packetID value. Type = "+QString::number(type));
            }
            message = new SNPubrel(pubrelPacketID);
        } break;

        case SN_SUBSCRIBE:
        {
            SNFlags *subscribeFlags = SNFlags::decode(buffer->readChar(), type);
            bytesLeft--;
            int subscribePacketID = buffer->readShort();
            if (subscribePacketID == 0) {
                throw new QString(QString::number(subscribePacketID)+" is invalid packetID value. Type = "+QString::number(type));
            }
            bytesLeft -= 2;
            if (!SNValuesValidator::canReadData(*buffer, bytesLeft) || bytesLeft < 2) {
                throw new QString("invalid topic encoding. Type = "+QString::number(type));
            }

            Topic *topic = NULL;

            switch(subscribeFlags->getTopicType()) {
                case SN_NAMED_TOPIC_TYPE:
                {
                    QString subscribeTopicName = buffer->readStringWithLength(bytesLeft);
                    topic = new SNFullTopic(subscribeTopicName, subscribeFlags->getQos());
                } break;
                case SN_ID_TOPIC_TYPE:
                {
                    int subscribeTopicID = buffer->readShort();
                    if (!SNValuesValidator::validateTopicID(subscribeTopicID)) {
                        throw new QString(QString::number(subscribePacketID)+" is invalid topicID value. Type = "+QString::number(type));
                    }
                    topic = new SNIdentifierTopic(subscribeTopicID, subscribeFlags->getQos());
                } break;
                case SN_SHORT_TOPIC_TYPE:
                {
                    QString subscribeTopicShortName = buffer->readStringWithLength(bytesLeft);
                    topic = new SNShortTopic(subscribeTopicShortName, subscribeFlags->getQos());
                } break;
                default: break;
            }
            message = new SNSubscribe(subscribePacketID, topic, subscribeFlags->getQos());
        } break;

        case SN_SUBACK:
        {
            SNFlags *subackFlags = SNFlags::decode(buffer->readChar(), type);
            int subackTopicID = buffer->readShort();
            if (!SNValuesValidator::validateTopicID(subackTopicID)) {
                throw new QString(QString::number(subackTopicID)+" is invalid topicID value. Type = "+QString::number(type));
            }

            int subackPacketID = buffer->readShort();
            if (!SNValuesValidator::validatePacketID(subackPacketID)) {
                throw new QString(QString::number(subackPacketID)+" is invalid packetID value. Type = "+QString::number(type));
            }

            SNReturnCode code = (SNReturnCode)buffer->readChar();
            message = new SNSuback(subackTopicID, subackPacketID, code, subackFlags->getQos());
        } break;

        case SN_UNSUBSCRIBE:
        {
            SNFlags *unsubscribeFlags = SNFlags::decode(buffer->readChar(), type);
            bytesLeft--;
            int unsubscribePacketId = buffer->readShort();
            if (!SNValuesValidator::validatePacketID(unsubscribePacketId)) {
                throw new QString(QString::number(unsubscribePacketId)+" is invalid packetID value. Type = "+QString::number(type));
            }
            bytesLeft--;

            Topic *topic = NULL;

            switch(unsubscribeFlags->getTopicType()) {
                case SN_NAMED_TOPIC_TYPE:
                {
                    QString unsubscribeTopicName = buffer->readStringWithLength(bytesLeft);
                    topic = new SNFullTopic(unsubscribeTopicName, unsubscribeFlags->getQos());
                } break;
                case SN_ID_TOPIC_TYPE:
                {
                    int unsubscribeTopicID = buffer->readShort();
                    if (!SNValuesValidator::validateTopicID(unsubscribeTopicID)) {
                        throw new QString(QString::number(unsubscribeTopicID)+" is invalid topicID value. Type = "+QString::number(type));
                    }
                    topic = new SNIdentifierTopic(unsubscribeTopicID, unsubscribeFlags->getQos());
                } break;
                case SN_SHORT_TOPIC_TYPE:
                {
                    QString unsubscribeTopicShortName = buffer->readStringWithLength(bytesLeft);
                    topic = new SNShortTopic(unsubscribeTopicShortName, unsubscribeFlags->getQos());
                } break;
                default: break;
            }
            message = new SNUnsubscribe(unsubscribePacketId, topic);
        } break;

        case SN_UNSUBACK:
        {
            int unsubackPacketID = buffer->readShort();
            if (!SNValuesValidator::validatePacketID(unsubackPacketID)) {
                throw new QString(QString::number(unsubackPacketID)+" is invalid packetID value. Type = "+QString::number(type));
            }
            message = new SNUnsuback(unsubackPacketID);
        } break;

        case SN_PINGREQ:
        {
            QString pingreqClientID;
            if (bytesLeft > 0) {
                pingreqClientID = buffer->readStringWithLength(bytesLeft);
            }
            message = new SNPingreq(pingreqClientID);
        } break;

        case SN_PINGRESP:
        {
            message = new SNPingresp();
        } break;

        case SN_DISCONNECT:
        {
            int duration = 0;
            if (bytesLeft > 1) {
                duration = buffer->readShort();
            }
            message = new SNDisconnect(duration);
        } break;

        case SN_WILLTOPICUPD:
        {
            SNFullTopic *topic = NULL;
            bool willTopicUpdateRetain = false;
            if (bytesLeft > 0) {
                SNFlags *willFlags = SNFlags::decode(buffer->readChar(), type);
                willTopicUpdateRetain = willFlags->getIsRetain();
                bytesLeft--;
                QString willTopicValue = buffer->readStringWithLength(bytesLeft);
                topic = new SNFullTopic(willTopicValue, willFlags->getQos());
            }
            message = new SNWillTopicUpd(topic, willTopicUpdateRetain);
        } break;

        case SN_WILLTOPICRESP:
        {
            SNReturnCode code = (SNReturnCode)buffer->readChar();
            message = new SNWillTopicResp(code);
        } break;

        case SN_WILLMSGUPD:
        {
            if (!SNValuesValidator::canReadData(*buffer, bytesLeft)) {
                throw new QString("must contain content data. Type = "+QString::number(type));
            }
            QString willMsgUpdContentString = buffer->readStringWithLength(bytesLeft);
            QByteArray content = willMsgUpdContentString.toUtf8();
            message = new SNWillMsgUpd(content);
        } break;

        case SN_WILLMSGRESP:
        {
            SNReturnCode code = (SNReturnCode)buffer->readChar();
            message = new SNWillMsgResp(code);
        } break;

        case SN_ENCAPSULATED:
        {

            SNControls *control = SNControls::decode(buffer->readChar());
            bytesLeft--;
            QString wirelessNodeId = buffer->readStringWithLength(bytesLeft);

            Message *encapsulatedMessage = SNParser::decode(buffer->getByteArray());
            message = new SNEncapsulated(control->getRadius(), wirelessNodeId, encapsulatedMessage);
        } break;

    }

    if (buffer->getSize() < 0) {
        throw new QString("not all bytes have been read from buffer");
    }

    return message;
}

int SNParser::decodeContentLength(ByteArray *data)
{
    int length = 0;
    short firstLengthByte = data->readChar();
    if (firstLengthByte == threeOctetLengthSuffix) {
        length = data->readShort();
    } else {
        length = firstLengthByte;
    }
    return length;
}
