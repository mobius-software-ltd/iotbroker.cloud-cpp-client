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

#ifndef IOTENUMPROTOCOL_H
#define IOTENUMPROTOCOL_H

#include "iot-protocols/amqp/classes/enums/enumobject.h"

enum IotEnumProtocols : int
{
    MQTT_PROTOCOL = 1,
    MQTT_SN_PROTOCOL = 2,
    COAP_PROTOCOL = 3,
    AMQP_PROTOCOL = 4
};

static QString const MQTT_PROTOCOL_STRING = QString("MQTT");
static QString const MQTT_SN_PROTOCOL_STRING = QString("MQTT-SN");
static QString const COAP_PROTOCOL_STRING = QString("COAP");
static QString const AMQP_PROTOCOL_STRING = QString("AMQP");

class IotEnumProtocol : public EnumObject
{
private:

    int value;

public:
    IotEnumProtocol();
    IotEnumProtocol(int value);

    int getValue();
    QString getName();
};

#endif // IOTENUMPROTOCOL_H
