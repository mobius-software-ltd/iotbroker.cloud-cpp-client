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

CoAPMessage::CoAPMessage()
{
    this->version = 1;
    this->optionMap = QMap<CoAPOptionDefinitions, QList<QString> >();
}

CoAPMessage::CoAPMessage(int code, bool confirmableFlag, bool tokenFlag, QString payload) : CoAPMessage()
{
    this->code = code;
    this->type = (confirmableFlag == true) ? COAP_CONFIRMABLE_TYPE : COAP_NONCONFIRMABLE_TYPE;
    this->isTokenExist = tokenFlag;
    this->payload = payload;
    this->messageID = 0;
    this->token = 0;
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

void CoAPMessage::addOperation(CoAPOptionDefinitions option, QString value)
{
    QList<QString> values = this->optionMap.value(option);

    if (values.count() == 0) {
        values = QList<QString>();
    }

    values.append(value);
    this->optionMap.insert(option, values);
}



int CoAPMessage::getVersion() const
{
    return version;
}

CoAPTypes CoAPMessage::getType() const
{
    return type;
}

void CoAPMessage::setType(const CoAPTypes &value)
{
    type = value;
}

bool CoAPMessage::getIsTokenExist() const
{
    return isTokenExist;
}

void CoAPMessage::setIsTokenExist(bool value)
{
    isTokenExist = value;
}

int CoAPMessage::getToken() const
{
    return token;
}

void CoAPMessage::setToken(int value)
{
    token = value;
}

int CoAPMessage::getCode() const
{
    return code;
}

void CoAPMessage::setCode(const int &value)
{
    code = value;
}

int CoAPMessage::getMessageID() const
{
    return messageID;
}

void CoAPMessage::setMessageID(int value)
{
    messageID = value;
}

QString CoAPMessage::getPayload() const
{
    return payload;
}

void CoAPMessage::setPayload(const QString &value)
{
    payload = value;
}

bool CoAPMessage::getIsResponse() const
{
    return isResponse;
}

void CoAPMessage::setIsResponse(bool value)
{
    isResponse = value;
}

QMap<CoAPOptionDefinitions, QList<QString> > CoAPMessage::getOptionMap() const
{
    return optionMap;
}
