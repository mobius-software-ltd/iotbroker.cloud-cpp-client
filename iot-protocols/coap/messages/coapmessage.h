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

#ifndef COAPMESSAGE_H
#define COAPMESSAGE_H

#include "iot-protocols/classes/message.h"
#include "iot-protocols/coap/classes/coapenums.h"
#include <QString>
#include <QList>
#include <QMap>

class CoAPMessage : public Message
{

    int version;

    CoAPTypes type;

    bool isTokenExist;
    int token;

    int code;

    int messageID;
    QString payload;

    bool isResponse;

    QMap<CoAPOptionDefinitions, QList<QString>> optionMap;

public:
    CoAPMessage();
    CoAPMessage(int code, bool confirmableFlag, bool tokenFlag, QString payload);

    virtual int getLength();
    virtual int getType();
    virtual IotEnumProtocol *getProtocol();

    void addOperation(CoAPOptionDefinitions option, QString value);



    int getVersion() const;

    CoAPTypes getType() const;
    void setType(const CoAPTypes &value);

    bool getIsTokenExist() const;
    void setIsTokenExist(bool value);

    int getToken() const;
    void setToken(int value);

    int getCode() const;
    void setCode(const int &value);

    int getMessageID() const;
    void setMessageID(int value);

    QString getPayload() const;
    void setPayload(const QString &value);

    bool getIsResponse() const;
    void setIsResponse(bool value);

    QMap<CoAPOptionDefinitions, QList<QString> > getOptionMap() const;
};

#endif // COAPMESSAGE_H
