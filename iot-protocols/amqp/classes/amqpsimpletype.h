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

#ifndef AMQPSIMPLETYPE_H
#define AMQPSIMPLETYPE_H

#include <QUuid>
#include <QObject>
#include <QVariant>
#include <QDateTime>
#include "iot-protocols/amqp/classes/enums/amqptype.h"
#include "iot-protocols/amqp/classes/wrappers/amqpsymbol.h"
#include "iot-protocols/amqp/classes/wrappers/amqpdecimal.h"

class AMQPSimpleType : public QObject
{
    Q_OBJECT
private:

    AMQPTypes type;
    QVariant *object;

    static AMQPSimpleType *simpleTypeFromVariant(QVariant *value);
    static QVariant *wrapSimpleTypeValue(QVariant *value, AMQPTypes type);

public:
    AMQPSimpleType();
    AMQPSimpleType(AMQPTypes type, QVariant *object);

    AMQPTypes getType() const;
    void setType(const AMQPTypes &value);

    QVariant *getObject() const;
    void setObject(QVariant *value);

    static bool variantToBool(QVariant *value);
    static QVariant *boolToVariant(bool value);

    static short variantToUByte(QVariant *value);
    static QVariant *UByteToVariant(short value);

    static unsigned short variantToUShort(QVariant *value);
    static QVariant *UShortToVariant(unsigned short value);

    static unsigned int variantToUInt(QVariant *value);
    static QVariant *UIntToVariant(unsigned int value);

    static unsigned long variantToULong(QVariant *value);
    static QVariant *ULongToVariant(unsigned long value);

    static char variantToChar(QVariant *value);
    static QVariant *charToVariant(char value);

    static short variantToShort(QVariant *value);
    static QVariant *shortToVariant(short value);

    static int variantToInt(QVariant *value);
    static QVariant *intToVariant(int value);

    static long variantToLong(QVariant *value);
    static QVariant *longToVariant(long value);

    static float variantToFloat(QVariant *value);
    static QVariant *floatToVariant(float value);

    static double variantToDouble(QVariant *value);
    static QVariant *doubleToVariant(double value);

    static QDateTime variantToTimestamp(QVariant *value);
    static QVariant *timestampToVariant(QDateTime value);

    static QUuid variantToUuid(QVariant *value);
    static QVariant *uuidToVariant(QUuid value);

    static QByteArray variantToBinary(QVariant *value);
    static QVariant *binaryToVariant(QByteArray value);

    static QString variantToString(QVariant *value);
    static QVariant *stringToVariant(QString value);

    static AMQPSymbol *variantToSymbol(QVariant *value);
    static QVariant *symbolToVariant(AMQPSymbol *value);

    static AMQPDecimal *variantToDecimal(QVariant *value);
    static QVariant *decimalToVariant(AMQPDecimal *value);

};

#endif // AMQPSIMPLETYPE_H
