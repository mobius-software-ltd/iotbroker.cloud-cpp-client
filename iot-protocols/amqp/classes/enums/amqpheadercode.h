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

#ifndef AMQPHEADERCODE_H
#define AMQPHEADERCODE_H

#include <QString>
#include "iot-protocols/amqp/classes/enums/enumobject.h"

enum AMQPHeaderCodes
{
    AMQP_PROTOCOL_HEADER_CODE       = 0x08,
    AMQP_OPEN_HEADER_CODE           = 0x10,
    AMQP_BEGIN_HEADER_CODE          = 0x11,
    AMQP_ATTACH_HEADER_CODE         = 0x12,
    AMQP_FLOW_HEADER_CODE           = 0x13,
    AMQP_TRANSFER_HEADER_CODE       = 0x14,
    AMQP_DISPOSITION_HEADER_CODE    = 0x15,
    AMQP_DETACH_HEADER_CODE         = 0x16,
    AMQP_END_HEADER_CODE            = 0x17,
    AMQP_CLOSE_HEADER_CODE          = 0x18,
    AMQP_MECHANISMS_HEADER_CODE     = 0x40,
    AMQP_INIT_HEADER_CODE           = 0x41,
    AMQP_CHALLENGE_HEADER_CODE      = 0x42,
    AMQP_RESPONSE_HEADER_CODE       = 0x43,
    AMQP_OUTCOME_HEADER_CODE        = 0x44,
    AMQP_PING_HEADER_CODE           = 0x00,
};

static QString const AMQP_PROTOCOL_HEADER_CODE_STRING       = QString("Protocol header");
static QString const AMQP_OPEN_HEADER_CODE_STRING           = QString("Open");
static QString const AMQP_BEGIN_HEADER_CODE_STRING          = QString("Begin");
static QString const AMQP_ATTACH_HEADER_CODE_STRING         = QString("Attach");
static QString const AMQP_FLOW_HEADER_CODE_STRING           = QString("Flow");
static QString const AMQP_TRANSFER_HEADER_CODE_STRING       = QString("Transfer");
static QString const AMQP_DISPOSITION_HEADER_CODE_STRING    = QString("Disposition");
static QString const AMQP_DETACH_HEADER_CODE_STRING         = QString("Detach");
static QString const AMQP_END_HEADER_CODE_STRING            = QString("End");
static QString const AMQP_CLOSE_HEADER_CODE_STRING          = QString("Close");
static QString const AMQP_MECHANISMS_HEADER_CODE_STRING     = QString("Mechanisms");
static QString const AMQP_INIT_HEADER_CODE_STRING           = QString("Init");
static QString const AMQP_CHALLENGE_HEADER_CODE_STRING      = QString("Challenge");
static QString const AMQP_RESPONSE_HEADER_CODE_STRING       = QString("Response");
static QString const AMQP_OUTCOME_HEADER_CODE_STRING        = QString("Outcome");
static QString const AMQP_PING_HEADER_CODE_STRING           = QString("Ping");

class AMQPHeaderCode : public EnumObject
{
private:

    int value;

public:
    AMQPHeaderCode();
    AMQPHeaderCode(int value);

    int getValue();
    QString getName();
};

#endif // AMQPHEADERCODE_H
