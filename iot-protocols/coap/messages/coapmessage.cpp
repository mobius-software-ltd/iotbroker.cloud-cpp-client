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

#include "coapmessage.h"
#include <algorithm>
#include <QDebug>
CoAPMessage::CoAPMessage()
{
    this->version = 1;
    this->messageID = 0;
    this->token = -1;
    this->options = QList<CoapOption>();
}

CoAPMessage::CoAPMessage(int version, CoAPTypes type, CoAPCode code, short messageId, long token, QList<CoapOption> options, QByteArray payload)
{
    this->version = version;
    this->type = type;
    this->code = code;
    this->messageID = messageId;
    this->token = token;
    this->options = options;
    this->payload = payload;
}

CoAPMessage::CoAPMessage(CoAPCode code, bool confirmableFlag, QString payload) : CoAPMessage()
{
    this->code = code;
    this->type = (confirmableFlag == true) ? COAP_CONFIRMABLE_TYPE : COAP_NONCONFIRMABLE_TYPE;
    this->payload = payload.toUtf8();
}

int CoAPMessage::getLength()
{
    return 0;
}

int CoAPMessage::getType()
{
    return this->type;
}

IotEnumProtocol *CoAPMessage::getProtocol()
{
    return new IotEnumProtocol(COAP_PROTOCOL);
}

void CoAPMessage::addOption(CoAPOptionDefinitions type, QByteArray data)
{
    CoapOption option = CoapOption(type, data.length(), data);
    this->options.append(option);
}

void CoAPMessage::addOption(CoAPOptionDefinitions type, QString string)
{
    this->addOption(type, string.toUtf8());
}

void CoAPMessage::addOption(CoapOption option)
{
    this->options.append(option);
}

CoapOption CoAPMessage::getOption(CoAPOptionDefinitions type)
{
    for (int i = 0; i < this->options.size(); i++) {
        CoapOption option = this->options.at(i);
        if (option.getNumber() == type) {
            return option;
        }
    }
    return CoapOption(0, 0, QByteArray());
}

QList<CoapOption> CoAPMessage::getOptions()
{
    std::sort(this->options.begin(), this->options.end(),CoapOption::numberLessThan);
    return options;
}

QString CoAPMessage::toString()
{
    return "version: " + QString::number(this->version) + "\n" +
            "type: " + QString::number(this->type) + "\n" +
            "token: " + QString::number(this->token) + "\n" +
            "code: " + QString::number(this->code) + "\n" +
            "messageID: " + QString::number(this->messageID) + "\n" +
            "payload: " + QString(this->payload) + "\n" +
            "options size: " + QString::number(this->options.size()) + "\n";
}


int CoAPMessage::getVersion() const
{
    return version;
}

void CoAPMessage::setVersion(int value)
{
    version = value;
}

void CoAPMessage::setType(const CoAPTypes &value)
{
    type = value;
}

long CoAPMessage::getToken() const
{
    return token;
}

void CoAPMessage::setToken(const long &value)
{
    token = value;
}

CoAPCode CoAPMessage::getCode() const
{
    return code;
}

void CoAPMessage::setCode(const CoAPCode &value)
{
    code = value;
}

short CoAPMessage::getMessageID() const
{
    return messageID;
}

void CoAPMessage::setMessageID(short value)
{
    messageID = value;
}

QByteArray CoAPMessage::getPayload() const
{
    return payload;
}

void CoAPMessage::setPayload(const QByteArray &value)
{
    payload = value;
}
