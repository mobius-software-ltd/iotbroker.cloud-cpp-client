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

#ifndef AMQPWRAPPER_H
#define AMQPWRAPPER_H

#include "iot-protocols/amqp/classes/tlv/api/tlvamqp.h"
#include "iot-protocols/amqp/classes/wrappers/amqpdecimal.h"
#include "iot-protocols/amqp/classes/wrappers/amqpsymbol.h"
#include "iot-protocols/amqp/classes/amqpsimpletype.h"
#include "iot-protocols/amqp/classes/tlv/array/amqptlvarray.h"
#include "iot-protocols/amqp/classes/tlv/compound/amqptlvlist.h"
#include "iot-protocols/amqp/classes/tlv/compound/amqptlvmap.h"
#include "iot-protocols/amqp/classes/tlv/variable/amqptlvvariable.h"
#include "iot-protocols/amqp/classes/tlv/fixed/amqptlvfixed.h"
#include "iot-protocols/amqp/classes/tlv/fixed/amqptlvnull.h"
#include <QDateTime>
#include <QUuid>
#include <QVariant>

class AMQPWrapper
{
private:

    static ByteArray convertUInt(unsigned int number);
    static ByteArray convertInt(int number);
    static ByteArray convertULong(unsigned long number);
    static ByteArray convertLong(long number);

public:

    static TLVAMQP *wrapObject(QVariant *object);
    static TLVAMQP *wrapBool(bool value);
    static TLVAMQP *wrapUByte(short value);
    static TLVAMQP *wrapByte(char value);
    static TLVAMQP *wrapUInt(unsigned int value);
    static TLVAMQP *wrapInt(int value);
    static TLVAMQP *wrapULong(unsigned long value);
    static TLVAMQP *wrapLong(long value);

    static AMQPTLVVariable *wrapBinary(QByteArray value);

    static TLVAMQP *wrapUUID(QUuid uuid);
    static TLVAMQP *wrapUShort(unsigned short value);
    static TLVAMQP *wrapShort(short value);
    static TLVAMQP *wrapDouble(double value);
    static TLVAMQP *wrapFloat(float value);
    static TLVAMQP *wrapChar(char value);
    static TLVAMQP *wrapTimestamp(QDateTime value);
    static TLVAMQP *wrapDecimal32(AMQPDecimal *value);
    static TLVAMQP *wrapDecimal64(AMQPDecimal *value);
    static TLVAMQP *wrapDecimal128(AMQPDecimal *value);

    static AMQPTLVVariable *wrapString(QString value);
    static AMQPTLVVariable *wrapSymbol(AMQPSymbol *value);
    static AMQPTLVList *wrapList(QList<QVariant *> value);
    static AMQPTLVMap *wrapMap(QMap<QVariant *, QVariant *> value);
    static AMQPTLVArray *wrapArray(QList<QVariant *> value);

};

#endif // AMQPWRAPPER_H
