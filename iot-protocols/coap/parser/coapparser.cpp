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

#include "coapparser.h"
#include <QDebug>

static int const CoAP_13_Optional_Faild_Constant = 13;
static int const CoAP_14_Optional_Faild_Constant = 14;
static int const CoAP_15_Optional_Faild_Constant = 15;

CoAPParser::CoAPParser()
{

}

Message *CoAPParser::decode(QByteArray data)
{
    ByteArray *buffer = new ByteArray(data);

    int size = buffer->getSize();

    CoAPMessage *message = new CoAPMessage();

    int zeroByte = buffer->readChar();
    int version = (zeroByte >> 6) & 0x3;

    if (version != message->getVersion()) {
        return NULL;
    }

    CoAPTypes type = (CoAPTypes)((zeroByte >> 4) & 0x3);
    message->setType(type);
    int tokenLength = (zeroByte >> 0) & 0xF;

    message->setIsTokenExist((tokenLength == 0) ? false : true);
    message->setCode((CoAPMethods)buffer->readChar());

    message->setMessageID(buffer->numberWithLength(2));

    if (message->getIsTokenExist()) {
        message->setToken(buffer->numberWithLength(tokenLength));
    }

    int previousOptionDelta = 0;

    while (buffer->getSize() > 0) {

        char optionByte = buffer->readChar();
        char optionDelta = (optionByte >> 4) & 0xF;
        char optionLength = optionByte & 0xF;

        if (optionDelta == CoAP_15_Optional_Faild_Constant) {
            if (optionLength != CoAP_15_Optional_Faild_Constant) {
                return NULL;
            }
            break;
        }

        int extendedDelta = 0;
        char optionIndexOffset = 1;

        if (optionDelta == CoAP_13_Optional_Faild_Constant) {
            optionIndexOffset += 1;
        } else if (optionDelta == CoAP_14_Optional_Faild_Constant) {
            optionIndexOffset += 2;
        }

        if ((size - buffer->getSize()) + optionIndexOffset <= size) {
            extendedDelta = buffer->numberWithLength(optionIndexOffset - 1);
        } else {
            return NULL;
        }

        int optionLengthExtendedOffsetIndex = optionIndexOffset;

        if (optionLength == CoAP_13_Optional_Faild_Constant) {
            optionIndexOffset += 1;
        } else if (optionLength == CoAP_14_Optional_Faild_Constant) {
            optionIndexOffset += 2;
        } else if (optionLength == CoAP_15_Optional_Faild_Constant) {
            return NULL;
        }
        optionLength += buffer->numberWithLength(optionIndexOffset - optionLengthExtendedOffsetIndex);

        if ((size - buffer->getSize()) + optionIndexOffset + optionLength > size) {
            return NULL;
        }

        int newOptionNumber = optionDelta + extendedDelta + previousOptionDelta;
        QString optionValue = buffer->readStringWithLength(optionLength);
        message->addOperation((CoAPOptionDefinitions)newOptionNumber, optionValue);
        previousOptionDelta += optionDelta + extendedDelta;
    }

    if ((size - buffer->getSize()) < size) {
        int length = buffer->getSize();
        message->setPayload(buffer->readStringWithLength(length));
    }

    return message;
}

QByteArray CoAPParser::encode(Message *message)
{
    CoAPMessage *msg = (CoAPMessage *)message;

    QString *final = new QString();
    QString tokerAsString = this->hexStringFromInt((int)msg->getToken());

    int zeroByte = 0;

    zeroByte |= (01 << 6);
    zeroByte |= (message->getType() << 4);
    zeroByte |= tokerAsString.length() / 2;

    final->append(QString().sprintf("%02X", zeroByte));
    final->append(QString().sprintf("%02lX", (unsigned long)msg->getCode()));
    final->append(QString().sprintf("%04lX", (unsigned long)msg->getMessageID()));
    final->append(tokerAsString);

    qDebug() << " = "<< *final;

    QList<CoAPOptionDefinitions> sortedArray = msg->getOptionMap().keys();

    uint previousDelta = 0;

    for (int i = 0; i < sortedArray.length(); i++) {
        CoAPOptionDefinitions key = sortedArray.at(i);

        QList<QString> valueArray = msg->getOptionMap().value(key);

        qDebug() << "VALUE == " << msg->getOptionMap().value(key);


        for (int i = 0; i < valueArray.count(); i++) {

            uint delta = key - previousDelta;
            QString valueForKey = QString();

            if (key == COAP_ETAG_OPTION || key == COAP_IF_MATCH_OPTION) {
                valueForKey = valueArray.at(i);
            } else if (key == COAP_BLOCK2_OPTION    || key == COAP_URI_PORT_OPTION  || key == COAP_CONTENT_FORMAT_OPTION    ||
                       key == COAP_MAX_AGE_OPTION   || key == COAP_ACCEPT_OPTION    || key == COAP_SIZE1_OPTION             ||
                       key == COAP_SIZE2_OPTION     || key == COAP_OBSERVE_OPTION) {
                valueForKey = this->hexStringFromInt(valueArray.at(i).toInt());
            } else {
                valueForKey = this->hexStringFromString(valueArray.at(i));
            }

            int length = (int)(valueForKey.length() / 2);

            QString extendedDelta = QString();
            QString extendedLength = QString();

            if (delta >= 269) {
                final->append(QString().sprintf("%01X", CoAP_14_Optional_Faild_Constant));
                extendedDelta = QString().sprintf("%04X", delta - 269);
            } else if (delta >= 13) {
                final->append(QString().sprintf("%01X", CoAP_13_Optional_Faild_Constant));
                extendedDelta = QString().sprintf("%02X", delta - CoAP_13_Optional_Faild_Constant);
            } else {
                final->append(QString().sprintf("%01X", delta));
            }

            if (length >= 269) {
                final->append(QString().sprintf("%01X", CoAP_14_Optional_Faild_Constant));
                extendedLength = QString().sprintf("%04X", length - 269);
            } else if (length >= CoAP_13_Optional_Faild_Constant) {
                final->append(QString().sprintf("%01X", CoAP_13_Optional_Faild_Constant));
                extendedLength = QString().sprintf("%02X", length - CoAP_13_Optional_Faild_Constant);
            } else {
                final->append(QString().sprintf("%01X", length));
            }

            final->append(extendedDelta);
            final->append(extendedLength);
            final->append(valueForKey);

            previousDelta += delta;
        }
    }

    if (msg->getPayload().length() > 0) {
        if (this->payloadDecodeForMessage(msg)) {
            final->append(QString().sprintf("%02X", 255));
            final->append(this->hexStringFromString(msg->getPayload()));
        } else {
            final->append(QString().sprintf("%02X", 255));
            final->append(msg->getPayload());
        }
    }

    return this->getHexDataFromString(final);
}

// private

QString CoAPParser::hexStringFromInt(int value)
{
    QString string = NULL;

    if (value == 0) {
        string = QString();
    } else if (value < 255) {
        string = QString().sprintf("%02X", value);
    } else if (value <= 65535) {
        string = QString().sprintf("%04X", value);
    } else if (value <= 16777215) {
        string = QString().sprintf("%06X", value);
    } else {
        string = QString().sprintf("%08X", value);
    }

    return string;
}

bool CoAPParser::payloadDecodeForMessage(Message *message)
{
    CoAPMessage *msg = (CoAPMessage *)message;

    if (msg->getOptionMap().value(COAP_CONTENT_FORMAT_OPTION).count()) {
        QList<QString> values = msg->getOptionMap().value(COAP_CONTENT_FORMAT_OPTION);
        if (values.at(0).toInt() == COAP_PLAIN_CONTENT_FORMAT ||
                values.at(0).toInt() == COAP_LINK_CONTENT_FORMAT ||
                values.at(0).toInt() == COAP_XML_CONTENT_FORMAT ||
                values.at(0).toInt() == COAP_JSON_CONTENT_FORMAT) {
            return true;
        }
    } else {
        return true;
    }
    return false;
}

QByteArray CoAPParser::getHexDataFromString(QString *string)
{
    QByteArray commandData = QByteArray();
    char byteRepresentation;
    char byte_chars[3] = {'\0','\0','\0'};

    QByteArray data = QByteArray(string->toUtf8());

    for (int i = 0; i < data.length() / 2; i++) {
        byte_chars[0] = data.at(i * 2);
        byte_chars[1] = data.at(i * 2 + 1);
        byteRepresentation = strtol(byte_chars, NULL, 16);
        commandData.append(byteRepresentation);
    }
    return commandData;
}

QString CoAPParser::hexStringFromString(QString string)
{
    return this->stringFromDataWithHex(string.toUtf8());
}

QString CoAPParser::stringFromDataWithHex(QByteArray data)
{
    return data.toHex();
}
