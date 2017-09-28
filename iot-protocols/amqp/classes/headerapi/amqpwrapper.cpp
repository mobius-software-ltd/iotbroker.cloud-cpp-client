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

#include "amqpwrapper.h"
#include <QDebug>

TLVAMQP *AMQPWrapper::wrapObject(QVariant *object)
{
    if (object == NULL) {
        return new AMQPTLVNull();
    }

    TLVAMQP *result = NULL;

    AMQPSimpleType *type = (AMQPSimpleType *)object->value<AMQPSimpleType *>();

    switch (type->getType()) {
        case AMQP_BOOLEAN_TYPE:
        {
            result = AMQPWrapper::wrapBool(AMQPSimpleType::variantToBool(object));
        }  break;
        case AMQP_UBYTE_TYPE:
        {
            result = AMQPWrapper::wrapUByte(AMQPSimpleType::variantToUByte(object));
        }  break;
        case AMQP_USHORT_TYPE:
        case AMQP_UINT_TYPE:
        case AMQP_SMALL_UINT_TYPE:
        case AMQP_UINT0_TYPE:
        {
            result = AMQPWrapper::wrapUShort(AMQPSimpleType::variantToUShort(object));
        }  break;
        case AMQP_ULONG_TYPE:
        case AMQP_SMALL_ULONG_TYPE:
        case AMQP_ULONG0_TYPE:
        {
            result = AMQPWrapper::wrapUInt(AMQPSimpleType::variantToUInt(object));
        }  break;
        case AMQP_BYTE_TYPE:
        {
            result = AMQPWrapper::wrapByte(AMQPSimpleType::variantToChar(object));
        }  break;
        case AMQP_SHORT_TYPE:
        {
            result = AMQPWrapper::wrapShort(AMQPSimpleType::variantToShort(object));
        }  break;
        case AMQP_INT_TYPE:
        case AMQP_SMALL_INT_TYPE:
        {
            result = AMQPWrapper::wrapInt(AMQPSimpleType::variantToInt(object));
        }  break;
        case AMQP_LONG_TYPE:
        case AMQP_SMALL_LONG_TYPE:
        {
            result = AMQPWrapper::wrapLong(AMQPSimpleType::variantToLong(object));
        }  break;
        case AMQP_FLOAT_TYPE:
        {
            result = AMQPWrapper::wrapFloat(AMQPSimpleType::variantToFloat(object));
        }  break;
        case AMQP_DOUBLE_TYPE:
        {
            result = AMQPWrapper::wrapDouble(AMQPSimpleType::variantToDouble(object));
        }  break;
        case AMQP_CHAR_TYPE:
        {
            result = AMQPWrapper::wrapChar(AMQPSimpleType::variantToChar(object));
        }
        case AMQP_DECIMAL32_TYPE:
        case AMQP_DECIMAL64_TYPE:
        case AMQP_DECIMAL128_TYPE:
        {        
            AMQPDecimal *decimal = AMQPSimpleType::variantToDecimal(object);
            if (decimal->getValue().getSize() == 4) {
                result = AMQPWrapper::wrapDecimal32(decimal);
            } else if (decimal->getValue().getSize() == 8) {
                result = AMQPWrapper::wrapDecimal64(decimal);
            } else if (decimal->getValue().getSize() == 16) {
                result = AMQPWrapper::wrapDecimal128(decimal);
            }
        }  break;
        case AMQP_TIMESTAMP_TYPE:
        {
            result = AMQPWrapper::wrapTimestamp(AMQPSimpleType::variantToTimestamp(object));
        }  break;
        case AMQP_UUID_TYPE:
        {
            result = AMQPWrapper::wrapUUID(AMQPSimpleType::variantToUuid(object));
        }  break;
        case AMQP_BINARY8_TYPE:
        case AMQP_BINARY32_TYPE:
        {
            result = AMQPWrapper::wrapBinary(AMQPSimpleType::variantToBinary(object));
        }  break;
        case AMQP_STRING8_TYPE:
        case AMQP_STRING32_TYPE:
        {
            result = AMQPWrapper::wrapString(AMQPSimpleType::variantToString(object));
        }  break;
        case AMQP_SYMBOL8_TYPE:
        case AMQP_SYMBOL32_TYPE:
        {
            result = AMQPWrapper::wrapSymbol(AMQPSimpleType::variantToSymbol(object));
        }  break;
        default: qDebug() << "UNWRAPPER: wrong type!"; break;
    }

    return result;
}

TLVAMQP *AMQPWrapper::wrapBool(bool value)
{
    ByteArray data = ByteArray();
    AMQPType *type = new AMQPType(value ? AMQP_BOOLEAN_TRUE_TYPE : AMQP_BOOLEAN_FALSE_TYPE);
    return new AMQPTLVFixed(type, data);
}

TLVAMQP *AMQPWrapper::wrapUByte(short value)
{
    if (value < 0) {
        qDebug() << "AMQPWrapper::wrapUByte";
        return NULL;
    }

    ByteArray data = ByteArray();
    AMQPType *type = new AMQPType(AMQP_UBYTE_TYPE);
    data.writeChar(value);
    return new AMQPTLVFixed(type, data);
}

TLVAMQP *AMQPWrapper::wrapByte(char value)
{
    ByteArray data = ByteArray();
    AMQPType *type = new AMQPType(AMQP_BYTE_TYPE);
    data.writeChar(value);
    return new AMQPTLVFixed(type, data);
}

TLVAMQP *AMQPWrapper::wrapUInt(unsigned int value)
{
    ByteArray data = AMQPWrapper::convertUInt(value);
    AMQPType *type = NULL;

    if (data.getSize() == 0) {
        type = new AMQPType(AMQP_UINT0_TYPE);
    } else if (data.getSize() == 1) {
        type = new AMQPType(AMQP_SMALL_UINT_TYPE);
    } else if (data.getSize() > 1) {
        type = new AMQPType(AMQP_UINT_TYPE);
    }
    return new AMQPTLVFixed(type, data);
}

TLVAMQP *AMQPWrapper::wrapInt(int value)
{
    ByteArray data = AMQPWrapper::convertInt(value);
    AMQPTypes tp = (data.getSize() > 1) ? AMQP_INT_TYPE : AMQP_SMALL_INT_TYPE;
    AMQPType *type = new AMQPType(tp);
    return new AMQPTLVFixed(type, data);
}

TLVAMQP *AMQPWrapper::wrapULong(unsigned long value)
{
    ByteArray data = AMQPWrapper::convertULong(value);
    AMQPType *type = NULL;

    if (data.getSize() == 0) {
        type = new AMQPType(AMQP_ULONG0_TYPE);
    } else if (data.getSize() == 1) {
        type = new AMQPType(AMQP_SMALL_ULONG_TYPE);
    } else if (data.getSize() > 1) {
        type = new AMQPType(AMQP_ULONG_TYPE);
    }
    return new AMQPTLVFixed(type, data);
}

TLVAMQP *AMQPWrapper::wrapLong(long value)
{
    ByteArray data = AMQPWrapper::convertLong(value);
    AMQPTypes tp = (data.getSize() > 1) ? AMQP_LONG_TYPE : AMQP_SMALL_LONG_TYPE;
    AMQPType *type = new AMQPType(tp);
    return new AMQPTLVFixed(type, data);
}

AMQPTLVVariable *AMQPWrapper::wrapBinary(QByteArray value)
{
    if (value.isNull()) {
        qDebug() << "AMQPWrapper::wrapBinary";
        return NULL;
    }

    AMQPTypes tp = (value.length() > 255) ? AMQP_BINARY32_TYPE : AMQP_BINARY8_TYPE;
    AMQPType *type = new AMQPType(tp);
    return new AMQPTLVVariable(type, ByteArray(value));
}

TLVAMQP *AMQPWrapper::wrapUUID(QUuid uuid)
{
    if (uuid.isNull()) {
        qDebug() << "AMQPWrapper::wrapUUID";
        return NULL;
    }

    AMQPType *type = new AMQPType(AMQP_UUID_TYPE);
    QByteArray data = uuid.toByteArray();
    return new AMQPTLVFixed(type, ByteArray(data));
}

TLVAMQP *AMQPWrapper::wrapUShort(unsigned short value)
{
    if (value < 0) {
        qDebug() << "AMQPWrapper::wrapUShort";
        return NULL;
    }

    AMQPType *type = new AMQPType(AMQP_USHORT_TYPE);
    ByteArray data = ByteArray();
    data.writeShort(value);
    return new AMQPTLVFixed(type, ByteArray(data));
}

TLVAMQP *AMQPWrapper::wrapShort(short value)
{
    AMQPType *type = new AMQPType(AMQP_SHORT_TYPE);
    ByteArray data = ByteArray();
    data.writeShort(value);
    return new AMQPTLVFixed(type, ByteArray(data));
}

TLVAMQP *AMQPWrapper::wrapDouble(double value)
{
    AMQPType *type = new AMQPType(AMQP_DOUBLE_TYPE);
    ByteArray data = ByteArray();
    data.writeDouble(value);
    return new AMQPTLVFixed(type, ByteArray(data));
}

TLVAMQP *AMQPWrapper::wrapFloat(float value)
{
    AMQPType *type = new AMQPType(AMQP_FLOAT_TYPE);
    ByteArray data = ByteArray();
    data.writeFloat(value);
    return new AMQPTLVFixed(type, ByteArray(data));
}

TLVAMQP *AMQPWrapper::wrapChar(char value)
{
    AMQPType *type = new AMQPType(AMQP_CHAR_TYPE);
    ByteArray data = ByteArray();
    data.writeInt(value);
    return new AMQPTLVFixed(type, ByteArray(data));
}

TLVAMQP *AMQPWrapper::wrapTimestamp(QDateTime value)
{
    if (value.isNull()) {
        qDebug() << "AMQPWrapper::wrapTimestamp";
        return NULL;
    }

    AMQPType *type = new AMQPType(AMQP_TIMESTAMP_TYPE);
    ByteArray data = ByteArray();
    data.writeLong(value.currentMSecsSinceEpoch());
    return new AMQPTLVFixed(type, ByteArray(data));
}

TLVAMQP *AMQPWrapper::wrapDecimal32(AMQPDecimal *value)
{
    if (value == NULL) {
        qDebug() << "AMQPWrapper::wrapDecimal32";
        return NULL;
    }

    AMQPType *type = new AMQPType(AMQP_DECIMAL32_TYPE);
    return new AMQPTLVFixed(type, value->getValue());
}

TLVAMQP *AMQPWrapper::wrapDecimal64(AMQPDecimal *value)
{
    if (value == NULL) {
        qDebug() << "AMQPWrapper::wrapDecimal64";
        return NULL;
    }

    AMQPType *type = new AMQPType(AMQP_DECIMAL64_TYPE);
    return new AMQPTLVFixed(type, value->getValue());
}

TLVAMQP *AMQPWrapper::wrapDecimal128(AMQPDecimal *value)
{
    if (value == NULL) {
        qDebug() << "AMQPWrapper::wrapDecimal128";
        return NULL;
    }

    AMQPType *type = new AMQPType(AMQP_DECIMAL128_TYPE);
    return new AMQPTLVFixed(type, value->getValue());
}

AMQPTLVVariable *AMQPWrapper::wrapString(QString value)
{
    if (value.isNull()) {
        qDebug() << "AMQPWrapper::wrapString";
        return NULL;
    }

    AMQPTypes tp = (value.length() > 255) ? AMQP_STRING32_TYPE : AMQP_STRING8_TYPE;
    AMQPType *type = new AMQPType(tp);
    return new AMQPTLVVariable(type, ByteArray(value.toUtf8()));
}

AMQPTLVVariable *AMQPWrapper::wrapSymbol(AMQPSymbol *value)
{
    if (value == NULL) {
        qDebug() << "AMQPWrapper::wrapSymbol";
        return NULL;
    }

    QByteArray bytes = QByteArray(value->getValue().toUtf8());
    AMQPTypes tp = (bytes.length() > 255) ? AMQP_SYMBOL32_TYPE : AMQP_SYMBOL8_TYPE;
    AMQPType *type = new AMQPType(tp);
    return new AMQPTLVVariable(type, ByteArray(bytes));
}

AMQPTLVList *AMQPWrapper::wrapList(QList<QVariant *> value)
{
    if (value.count() == 0) {
        qDebug() << "AMQPWrapper::wrapList";
        return NULL;
    }

    AMQPTLVList *list = new AMQPTLVList();
    foreach (QVariant *item, value) {
        list->addElement(AMQPWrapper::wrapObject(item));
    }
    return list;
}

AMQPTLVMap *AMQPWrapper::wrapMap(QMap<QVariant *, QVariant *> value)
{
    if (value.count() == 0) {
        qDebug() << "AMQPWrapper::wrapMap";
        return NULL;
    }

    AMQPTLVMap *map = new AMQPTLVMap();
    foreach (QVariant *key, value.keys()) {
        QVariant *vl = value.value(key);
        map->putElement(AMQPWrapper::wrapObject(key), AMQPWrapper::wrapObject(vl));
    }
    return map;
}

AMQPTLVArray *AMQPWrapper::wrapArray(QList<QVariant *> value)
{
    if (value.count() == 0) {
        qDebug() << "AMQPWrapper::wrapArray";
        return NULL;
    }

    AMQPTLVArray *array = new AMQPTLVArray();
    foreach (QVariant *item, value) {
        array->addElement(AMQPWrapper::wrapObject(item));
    }
    return array;
}

// private methods

ByteArray AMQPWrapper::convertUInt(unsigned int number)
{
    ByteArray data = ByteArray();

    if (number == 0) {
        return data;
    } else if (number > 0 && number <= 255) {
        data.writeChar(number);
        return data;
    } else {
        data.writeInt(number);
        return data;
    }
}

ByteArray AMQPWrapper::convertInt(int number)
{
    ByteArray data = ByteArray();

    if (number == 0) {
        return data;
    } else if (number >= -128 && number <= 127) {
        data.writeChar(number);
        return data;
    } else {
        data.writeInt(number);
        return data;
    }
}

ByteArray AMQPWrapper::convertULong(unsigned long number)
{
    ByteArray data = ByteArray();

    if (number == 0) {
        return data;
    } else if (number >= 0 && number <= 255) {
        data.writeChar(number);
        return data;
    } else {
        data.writeLong(number);
        return data;
    }
}

ByteArray AMQPWrapper::convertLong(long number)
{
    ByteArray data = ByteArray();

    if (number == 0) {
        return data;
    } else if (number >= -128 && number <= 127) {
        data.writeChar(number);
        return data;
    } else {
        data.writeLong(number);
        return data;
    }
}
