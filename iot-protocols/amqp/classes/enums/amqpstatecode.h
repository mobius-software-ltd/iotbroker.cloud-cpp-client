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

#ifndef AMQPSTATECODE_H
#define AMQPSTATECODE_H

#include <QString>
#include "iot-protocols/amqp/classes/enums/enumobject.h"

enum AMQPStateCodes
{
    AMQP_ACCEPTED_STATE_CODE = 0x23,
    AMQP_REJECTED_STATE_CODE = 0x24,
    AMQP_RELEASED_STATE_CODE = 0x25,
    AMQP_MODIFIED_STATE_CODE = 0x26,
    AMQP_RECEIVED_STATE_CODE = 0x27,
};

static QString const AMQP_ACCEPTED_STATE_CODE_STRING = QString("accepted");
static QString const AMQP_REJECTED_STATE_CODE_STRING = QString("rejected");
static QString const AMQP_RELEASED_STATE_CODE_STRING = QString("released");
static QString const AMQP_MODIFIED_STATE_CODE_STRING = QString("modified");
static QString const AMQP_RECEIVED_STATE_CODE_STRING = QString("received");

class AMQPStateCode : public EnumObject
{
private:

    int value;

public:
    AMQPStateCode();
    AMQPStateCode(int value);

    int getValue();
    QString getName();
};

#endif // AMQPSTATECODE_H
