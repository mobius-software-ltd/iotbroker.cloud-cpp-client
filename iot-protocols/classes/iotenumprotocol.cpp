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

#include "iotenumprotocol.h"

IotEnumProtocol::IotEnumProtocol()
{
    this->map = QMap<int, QString>();

    this->map.insert(MQTT_PROTOCOL, MQTT_PROTOCOL_STRING);
    this->map.insert(MQTT_SN_PROTOCOL, MQTT_SN_PROTOCOL_STRING);
    this->map.insert(COAP_PROTOCOL, COAP_PROTOCOL_STRING);
    this->map.insert(AMQP_PROTOCOL, AMQP_PROTOCOL_STRING);
    this->map.insert(WEBSOCKET, WEBSOCKET_PROTOCOL_STRING);
}

IotEnumProtocol::IotEnumProtocol(int value) : IotEnumProtocol()
{
    this->value = value;
}

int IotEnumProtocol::getValue()
{
    return this->value;
}

QString IotEnumProtocol::getName()
{
    return EnumObject::getName(this->value);
}
