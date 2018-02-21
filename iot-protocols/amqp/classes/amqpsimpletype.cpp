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

#include "amqpsimpletype.h"

AMQPSimpleType::AMQPSimpleType()
{

}

AMQPSimpleType::AMQPSimpleType(AMQPTypes type, QVariant *object)
{
    this->type = type;
    this->object = object;
}

AMQPTypes AMQPSimpleType::getType() const
{
    return type;
}

void AMQPSimpleType::setType(const AMQPTypes &value)
{
    type = value;
}

QVariant *AMQPSimpleType::getObject() const
{
    return object;
}

void AMQPSimpleType::setObject(QVariant *value)
{
    object = value;
}

bool AMQPSimpleType::variantToBool(QVariant *value)
{
    AMQPSimpleType *type = simpleTypeFromVariant(value);
    return type->getObject()->toBool();
}

QVariant *AMQPSimpleType::boolToVariant(bool value)
{
    QVariant *valueVariant = new QVariant(value);
    return wrapSimpleTypeValue(valueVariant, AMQP_BOOLEAN_TYPE);
}

short AMQPSimpleType::variantToUByte(QVariant *value)
{
    AMQPSimpleType *type = simpleTypeFromVariant(value);
    return type->getObject()->toChar().toLatin1();
}

QVariant *AMQPSimpleType::UByteToVariant(short value)
{
    QVariant *valueVariant = new QVariant(value);
    return wrapSimpleTypeValue(valueVariant, AMQP_UBYTE_TYPE);
}

unsigned short AMQPSimpleType::variantToUShort(QVariant *value)
{
    AMQPSimpleType *type = simpleTypeFromVariant(value);
    return type->getObject()->toInt();
}

QVariant *AMQPSimpleType::UShortToVariant(unsigned short value)
{
    QVariant *valueVariant = new QVariant(value);
    return wrapSimpleTypeValue(valueVariant, AMQP_USHORT_TYPE);
}

unsigned int AMQPSimpleType::variantToUInt(QVariant *value)
{
    AMQPSimpleType *type = simpleTypeFromVariant(value);
    return type->getObject()->toUInt();
}

QVariant *AMQPSimpleType::UIntToVariant(unsigned int value)
{
    QVariant *valueVariant = new QVariant(value);
    return wrapSimpleTypeValue(valueVariant, AMQP_UINT0_TYPE);
}

unsigned long AMQPSimpleType::variantToULong(QVariant *value)
{
    AMQPSimpleType *type = simpleTypeFromVariant(value);
    return type->getObject()->toULongLong();
}

QVariant *AMQPSimpleType::ULongToVariant(unsigned long value)
{
    QVariant *valueVariant = new QVariant((unsigned long long)value);
    return wrapSimpleTypeValue(valueVariant, AMQP_ULONG_TYPE);
}

char AMQPSimpleType::variantToChar(QVariant *value)
{
    AMQPSimpleType *type = simpleTypeFromVariant(value);
    return type->getObject()->toChar().toLatin1();
}

QVariant *AMQPSimpleType::charToVariant(char value)
{
    QVariant *valueVariant = new QVariant(value);
    return wrapSimpleTypeValue(valueVariant, AMQP_CHAR_TYPE);
}

short AMQPSimpleType::variantToShort(QVariant *value)
{
    AMQPSimpleType *type = simpleTypeFromVariant(value);
    return type->getObject()->toInt();
}

QVariant *AMQPSimpleType::shortToVariant(short value)
{
    QVariant *valueVariant = new QVariant(value);
    return wrapSimpleTypeValue(valueVariant, AMQP_SHORT_TYPE);
}

int AMQPSimpleType::variantToInt(QVariant *value)
{
    AMQPSimpleType *type = simpleTypeFromVariant(value);
    return type->getObject()->toInt();
}

QVariant *AMQPSimpleType::intToVariant(int value)
{
    QVariant *valueVariant = new QVariant(value);
    return wrapSimpleTypeValue(valueVariant, AMQP_INT_TYPE);
}

long AMQPSimpleType::variantToLong(QVariant *value)
{
    AMQPSimpleType *type = simpleTypeFromVariant(value);
    return type->getObject()->toLongLong();
}

QVariant *AMQPSimpleType::longToVariant(long value)
{
    QVariant *valueVariant = new QVariant((long long)value);
    return wrapSimpleTypeValue(valueVariant, AMQP_LONG_TYPE);
}

float AMQPSimpleType::variantToFloat(QVariant *value)
{
    AMQPSimpleType *type = simpleTypeFromVariant(value);
    return type->getObject()->toFloat();
}

QVariant *AMQPSimpleType::floatToVariant(float value)
{
    QVariant *valueVariant = new QVariant(value);
    return wrapSimpleTypeValue(valueVariant, AMQP_FLOAT_TYPE);
}

double AMQPSimpleType::variantToDouble(QVariant *value)
{
    AMQPSimpleType *type = simpleTypeFromVariant(value);
    return type->getObject()->toDouble();
}

QVariant *AMQPSimpleType::doubleToVariant(double value)
{
    QVariant *valueVariant = new QVariant(value);
    return wrapSimpleTypeValue(valueVariant, AMQP_DOUBLE_TYPE);
}

QDateTime AMQPSimpleType::variantToTimestamp(QVariant *value)
{
    AMQPSimpleType *type = simpleTypeFromVariant(value);
    return type->getObject()->toDateTime();
}

QVariant *AMQPSimpleType::timestampToVariant(QDateTime value)
{
    QVariant *valueVariant = new QVariant(value);
    return wrapSimpleTypeValue(valueVariant, AMQP_TIMESTAMP_TYPE);
}

QUuid AMQPSimpleType::variantToUuid(QVariant *value)
{
    AMQPSimpleType *type = simpleTypeFromVariant(value);
    return type->getObject()->toUuid();
}

QVariant *AMQPSimpleType::uuidToVariant(QUuid value)
{
    QVariant *valueVariant = new QVariant(value);
    return wrapSimpleTypeValue(valueVariant, AMQP_UUID_TYPE);
}

QByteArray AMQPSimpleType::variantToBinary(QVariant *value)
{
    AMQPSimpleType *type = simpleTypeFromVariant(value);
    return type->getObject()->toByteArray();
}

QVariant *AMQPSimpleType::binaryToVariant(QByteArray value)
{
    QVariant *valueVariant = new QVariant(value);
    return wrapSimpleTypeValue(valueVariant, AMQP_BINARY8_TYPE);
}

QString AMQPSimpleType::variantToString(QVariant *value)
{
    AMQPSimpleType *type = simpleTypeFromVariant(value);
    return type->getObject()->toString();
}

QVariant *AMQPSimpleType::stringToVariant(QString value)
{
    QVariant *valueVariant = new QVariant(value);
    return wrapSimpleTypeValue(valueVariant, AMQP_STRING8_TYPE);
}

AMQPSymbol *AMQPSimpleType::variantToSymbol(QVariant *value)
{
    AMQPSimpleType *type = simpleTypeFromVariant(value);
    if (type->getType() == AMQP_SYMBOL8_TYPE || type->getType() == AMQP_SYMBOL32_TYPE) {
        return type->getObject()->value<AMQPSymbol *>();
    }
    return new AMQPSymbol();
}

QVariant *AMQPSimpleType::symbolToVariant(AMQPSymbol *value)
{
    QVariant *valueVariant = new QVariant();
    valueVariant->setValue<AMQPSymbol *>(value);
    return wrapSimpleTypeValue(valueVariant, AMQP_SYMBOL8_TYPE);
}

AMQPDecimal *AMQPSimpleType::variantToDecimal(QVariant *value)
{
    AMQPSimpleType *type = simpleTypeFromVariant(value);
    if (type->getType() == AMQP_DECIMAL32_TYPE || type->getType() == AMQP_DECIMAL64_TYPE || type->getType() == AMQP_DECIMAL128_TYPE) {
        return type->getObject()->value<AMQPDecimal *>();
    }
    return new AMQPDecimal();
}

QVariant *AMQPSimpleType::decimalToVariant(AMQPDecimal *value)
{
    QVariant *valueVariant = new QVariant();
    valueVariant->setValue<AMQPDecimal *>(value);
    return wrapSimpleTypeValue(valueVariant, AMQP_DECIMAL32_TYPE);
}

// private methods

AMQPSimpleType *AMQPSimpleType::simpleTypeFromVariant(QVariant *value)
{
    AMQPSimpleType *type = value->value<AMQPSimpleType *>();
    return type;
}

QVariant *AMQPSimpleType::wrapSimpleTypeValue(QVariant *value, AMQPTypes type)
{
    AMQPSimpleType *sType = new AMQPSimpleType(type, value);
    QVariant *variant = new QVariant();
    variant->setValue<AMQPSimpleType *>(sType);
    return variant;
}
