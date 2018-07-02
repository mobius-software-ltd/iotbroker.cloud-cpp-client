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

#include "coapparser.h"
#include <QDebug>
#include "classes/convertor.h"

CoAPParser::CoAPParser()
{

}

Message *CoAPParser::decode(QByteArray data)
{
    ByteArray buf = ByteArray(data);
    CoAPMessage *message = new CoAPMessage();

    int firstByte = buf.readChar();

    int version = firstByte >> 6;
    if (version != 1) {
        throw QString("COAP: Invalid version: " + QString::number(version));
    }
    message->setVersion(version);

    int typeValue = (firstByte >> 4) & 3;
    message->setType((CoAPTypes)typeValue);

    int tokenLength = firstByte & 0xf;
    if (tokenLength > 8) {
        throw QString("COAP: Invalid token length: " + QString::number(tokenLength));
    }

    int codeByte = buf.readChar();
    int codeValue = (codeByte >> 5) * 100;
    codeValue += codeByte & 0x1F;
    CoAPCode code = (CoAPCode)codeValue;

    message->setCode(code);
    message->setMessageID(buf.readShort());

    if (tokenLength > 0) {
        QByteArray tokenBytes = buf.readRawData(tokenLength);
        QString token = QString(tokenBytes);
        message->setToken(token.toInt());
    }

    int number = 0;

    while (buf.getSize() > 0) {
        unsigned char nextByte = buf.readChar();
        if (nextByte == 0xFF) {
            break;
        }
        int delta = (nextByte >> 4) & 15;

        if (delta == 13) {
            delta = ((unsigned char)buf.readChar()) + 13;
        } else if (delta == 14) {
            delta = buf.readShort() + 269;
        } else if (delta > 14) {
            throw QString("COAP: Invalid option delta value: " + QString::number(delta));
        }

        number += delta;

        int optionLength = nextByte & 15;
        if (optionLength == 13) {
            optionLength = ((unsigned char)buf.readChar()) + 13;
        } else if (optionLength == 14) {
            optionLength = buf.readShort() + 269;
        } else if (optionLength > 14) {
            throw QString("COAP: Invalid option delta value: " + QString::number(optionLength));
        }

        QByteArray optionValue;
        if (optionLength > 0) {
            optionValue = buf.readRawData(optionLength);
        }

        message->addOption(CoapOption(number, optionLength, optionValue));
    }

    if (buf.getSize() > 0) {
        int size = buf.getInitSize() - buf.getSize();
        message->setPayload(buf.readStringWithLength(size).toUtf8());
    }

    return message;
}

QByteArray CoAPParser::encode(Message *m)
{
    CoAPMessage *message = (CoAPMessage *)m;
    ByteArray buf = ByteArray();

    char firstByte = 0;

    firstByte += message->getVersion() << 6;
    firstByte += message->getType() << 4;

    if (message->getToken() != - 1) {
        firstByte += QString::number(message->getToken()).length();
    }

    buf.writeChar(firstByte);

    int codeMsb = (message->getCode() / 100);
    int codeLsb = (unsigned char) (message->getCode() % 100);
    int codeByte = ((codeMsb << 5) + codeLsb);

    buf.writeChar(codeByte);
    buf.writeShort(message->getMessageID());

    if (message->getToken() != - 1) {
        buf.writeRawData(QString::number(message->getToken()).toUtf8());
    }

    int previousNumber = 0;
    for (int i = 0; i < message->getOptions().size(); i++) {
        CoapOption option = message->getOptions().at(i);
        int delta = option.getNumber() - previousNumber;
        int nextByte = 0;

        int extendedDelta = -1;
        if (delta < 13) {
            nextByte += delta << 4;
        } else {
            extendedDelta = delta;
            if (delta < 0xFF) {
                nextByte = 13 << 4;
            } else {
                nextByte = 14 << 4;
            }
        }

        int extendedLength = -1;
        if (option.getLength() < 13) {
            nextByte += option.getLength();
        } else {
            extendedLength = option.getLength();
            if (option.getLength() < 0xFF) {
                nextByte += 13;
            } else {
                nextByte += 14;
            }
        }

        buf.writeChar((unsigned char)(nextByte));

        if (extendedDelta != -1) {
            if (extendedDelta < 0xFF) {
                buf.writeChar((unsigned char)(extendedDelta - 13));
            } else {
                buf.writeShort(extendedDelta - 269);
            }
        }

        if (extendedLength != -1) {
            if (extendedLength < 0xFF) {
                buf.writeChar((unsigned char)(extendedLength - 13));
            } else {
                buf.writeShort(extendedLength - 269);
            }
        }

        buf.writeRawData(option.getValue());
        previousNumber = option.getNumber();
    }

    buf.writeChar((unsigned char)0xFF);

    if (!message->getPayload().isEmpty()) {
        buf.writeRawData(message->getPayload());
    }

    return buf.getByteArray();
}

