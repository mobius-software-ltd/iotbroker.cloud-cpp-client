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

#include "amqpunwrapper.h"
#include <QDebug>
#include "iot-protocols/amqp/classes/amqpsimpletype.h"

QVariant *AMQPUnwrapper::unwrap(TLVAMQP *value)
{
    switch (value->getTypeValue()) {
        case AMQP_NULL_TYPE:
        {
            return NULL;
        }  break;
        case AMQP_BOOLEAN_TYPE:
        case AMQP_BOOLEAN_TRUE_TYPE:
        case AMQP_BOOLEAN_FALSE_TYPE:
        {
            return AMQPSimpleType::boolToVariant(AMQPUnwrapper::unwrapBool(value));
        }  break;
        case AMQP_UBYTE_TYPE:
        {
            return AMQPSimpleType::UByteToVariant(AMQPUnwrapper::unwrapUByte(value));
        }  break;
        case AMQP_USHORT_TYPE:
        {
            return AMQPSimpleType::UShortToVariant(AMQPUnwrapper::unwrapUShort(value));
        }  break;
        case AMQP_UINT_TYPE:
        case AMQP_SMALL_UINT_TYPE:
        case AMQP_UINT0_TYPE:
        {
            return AMQPSimpleType::UIntToVariant(AMQPUnwrapper::unwrapUInt(value));
        }  break;
        case AMQP_ULONG_TYPE:
        case AMQP_SMALL_ULONG_TYPE:
        case AMQP_ULONG0_TYPE:
        {
            return AMQPSimpleType::ULongToVariant(AMQPUnwrapper::unwrapULong(value));
        }  break;
        case AMQP_BYTE_TYPE:
        {
            return AMQPSimpleType::charToVariant(AMQPUnwrapper::unwrapByte(value));
        }  break;
        case AMQP_SHORT_TYPE:
        {
            return AMQPSimpleType::shortToVariant(AMQPUnwrapper::unwrapShort(value));
        }  break;
        case AMQP_INT_TYPE:
        case AMQP_SMALL_INT_TYPE:
        {
            return AMQPSimpleType::intToVariant(AMQPUnwrapper::unwrapInt(value));
        }  break;
        case AMQP_LONG_TYPE:
        case AMQP_SMALL_LONG_TYPE:
        {
            return AMQPSimpleType::longToVariant(AMQPUnwrapper::unwrapLong(value));
        }  break;
        case AMQP_FLOAT_TYPE:
        {
            return AMQPSimpleType::floatToVariant(AMQPUnwrapper::unwrapFloat(value));
        }  break;
        case AMQP_DOUBLE_TYPE:
        {
            return AMQPSimpleType::doubleToVariant(AMQPUnwrapper::unwrapDouble(value));
        }  break;
        case AMQP_DECIMAL32_TYPE:
        case AMQP_DECIMAL64_TYPE:
        case AMQP_DECIMAL128_TYPE:
        {
            return AMQPSimpleType::decimalToVariant(AMQPUnwrapper::unwrapDecimal(value));
        }  break;
        case AMQP_CHAR_TYPE:
        {
            return AMQPSimpleType::charToVariant(AMQPUnwrapper::unwrapByte(value));
        }  break;
        case AMQP_TIMESTAMP_TYPE:
        {
            return AMQPSimpleType::timestampToVariant(AMQPUnwrapper::unwrapTimestamp(value));
        }  break;
        case AMQP_UUID_TYPE:
        {
            return AMQPSimpleType::uuidToVariant(AMQPUnwrapper::unwrapUUID(value));
        }  break;
        case AMQP_BINARY8_TYPE:
        case AMQP_BINARY32_TYPE:
        {
            return AMQPSimpleType::binaryToVariant(AMQPUnwrapper::unwrapBinary(value));
        }  break;
        case AMQP_STRING8_TYPE:
        case AMQP_STRING32_TYPE:
        {
            return AMQPSimpleType::stringToVariant(AMQPUnwrapper::unwrapString(value));
        }  break;
        case AMQP_SYMBOL8_TYPE:
        case AMQP_SYMBOL32_TYPE:
        {
            return AMQPSimpleType::symbolToVariant(AMQPUnwrapper::unwrapSymbol(value));
        }  break;
    default: break;
    }
    return NULL;
}

short AMQPUnwrapper::unwrapUByte(TLVAMQP *tlv)
{
    if (tlv->getTypeValue() != AMQP_UBYTE_TYPE) {
        qDebug() << "AMQPUnwrapper::unwrapUByte";
        return 0;
    }
    char *bytes = tlv->getValue().getByteArray().data();
    return (short)(bytes[0] & 0xff);
}

char AMQPUnwrapper::unwrapByte(TLVAMQP *tlv)
{
    if (tlv->getTypeValue() != AMQP_BYTE_TYPE) {
        qDebug() << "AMQPUnwrapper::unwrapByte";
        return 0;
    }
    char *bytes = tlv->getValue().getByteArray().data();
    return bytes[0];
}

int AMQPUnwrapper::unwrapUShort(TLVAMQP *tlv)
{
    if (tlv->getTypeValue() != AMQP_USHORT_TYPE) {
        qDebug() << "AMQPUnwrapper::unwrapUShort";
        return 0;
    }
    return (tlv->getValue().readShort() & 0xffff);
}

short AMQPUnwrapper::unwrapShort(TLVAMQP *tlv)
{
    if (tlv->getTypeValue() != AMQP_SHORT_TYPE) {
        qDebug() << "AMQPUnwrapper::unwrapShort";
        return 0;
    }
    return tlv->getValue().readShort();
}

long AMQPUnwrapper::unwrapUInt(TLVAMQP *tlv)
{
    if (tlv->getTypeValue() != AMQP_UINT0_TYPE &&
            tlv->getTypeValue() != AMQP_SMALL_UINT_TYPE &&
            tlv->getTypeValue() != AMQP_UINT_TYPE) {
        qDebug() << "AMQPUnwrapper::unwrapUInt";
        return 0;
    }

    if (tlv->getValue().getSize() == 0) {
        return 0;
    }
    if (tlv->getValue().getSize() == 1) {
        char *bytes = (char *)tlv->getValue().getByteArray().data();
        return (bytes[0] & 0xff);
    }
    return (tlv->getValue().readInt() & 0xffffffff);
}

int AMQPUnwrapper::unwrapInt(TLVAMQP *tlv)
{
    if (tlv->getTypeValue() != AMQP_INT_TYPE &&
            tlv->getTypeValue() != AMQP_SMALL_INT_TYPE) {
        qDebug() << "AMQPUnwrapper::unwrapInt";
        return 0;
    }

    if (tlv->getValue().getSize() == 0) {
        return 0;
    }
    if (tlv->getValue().getSize() == 1) {
        char *bytes = (char *)tlv->getValue().getByteArray().data();
        return bytes[0];
    }
    return tlv->getValue().readInt();
}

unsigned long AMQPUnwrapper::unwrapULong(TLVAMQP *tlv)
{
    if (tlv->getTypeValue() != AMQP_ULONG_TYPE &&
            tlv->getTypeValue() != AMQP_SMALL_ULONG_TYPE &&
            tlv->getTypeValue() != AMQP_ULONG0_TYPE) {
        qDebug() << "AMQPUnwrapper::unwrapULong";
        return 0;
    }

    if (tlv->getValue().getSize() == 0) {
        return 0;
    }
    if (tlv->getValue().getSize() == 1) {
        char *bytes = (char *)tlv->getValue().getByteArray().data();
        return bytes[0] & 0xff;
    }
    return tlv->getValue().readLong();
}

long AMQPUnwrapper::unwrapLong(TLVAMQP *tlv)
{
    if (tlv->getTypeValue() != AMQP_LONG_TYPE &&
            tlv->getTypeValue() != AMQP_SMALL_LONG_TYPE) {
        qDebug() << "AMQPUnwrapper::unwrapLong";
        return 0;
    }

    if (tlv->getValue().getSize() == 0) {
        return 0;
    }
    if (tlv->getValue().getSize() == 1) {
        char *bytes = (char *)tlv->getValue().getByteArray().data();
        return (long)bytes[0];
    }
    return tlv->getValue().readLong();
}

bool AMQPUnwrapper::unwrapBool(TLVAMQP *tlv)
{
    if (tlv->getTypeValue() == AMQP_BOOLEAN_TYPE) {
        char *bytes = (char *)tlv->getValue().getByteArray().data();
        char byte = bytes[0];
        if (byte == 0) {
            return false;
        } else if (byte == 1) {
            return true;
        }
    } else if (tlv->getTypeValue() == AMQP_BOOLEAN_TRUE_TYPE) {
        return true;
    } else if (tlv->getTypeValue() == AMQP_BOOLEAN_FALSE_TYPE) {
        return false;
    }
    return false;
}

double AMQPUnwrapper::unwrapDouble(TLVAMQP *tlv)
{
    if (tlv->getTypeValue() != AMQP_DOUBLE_TYPE) {
        qDebug() << "AMQPUnwrapper::unwrapDouble";
        return 0;
    }
    return tlv->getValue().readDouble();
}

float AMQPUnwrapper::unwrapFloat(TLVAMQP *tlv)
{
    if (tlv->getTypeValue() != AMQP_FLOAT_TYPE) {
        qDebug() << "AMQPUnwrapper::unwrapFloat";
        return 0;
    }
    return tlv->getValue().readFloat();
}

QDateTime AMQPUnwrapper::unwrapTimestamp(TLVAMQP *tlv)
{
    if (tlv->getTypeValue() != AMQP_TIMESTAMP_TYPE) {
        qDebug() << "AMQPUnwrapper::unwrapTimestamp";
        return QDateTime();
    }
    QDateTime dateTime = QDateTime();
    dateTime.setMSecsSinceEpoch(tlv->getValue().readLong());
    return dateTime;
}

AMQPDecimal *AMQPUnwrapper::unwrapDecimal(TLVAMQP *tlv)
{
    if (tlv->getTypeValue() != AMQP_DECIMAL32_TYPE &&
            tlv->getTypeValue() != AMQP_DECIMAL64_TYPE &&
            tlv->getTypeValue() != AMQP_DECIMAL128_TYPE) {
        qDebug() << "AMQPUnwrapper::unwrapDecimal";
        return NULL;
    }
    return new AMQPDecimal(tlv->getValue());
}

int AMQPUnwrapper::unwrapChar(TLVAMQP *tlv)
{
    if (tlv->getTypeValue() != AMQP_CHAR_TYPE) {
        qDebug() << "AMQPUnwrapper::unwrapChar";
        return 0;
    }
    return tlv->getValue().readInt();
}

QString AMQPUnwrapper::unwrapString(TLVAMQP *tlv)
{
    if (tlv->getTypeValue() != AMQP_STRING8_TYPE &&
            tlv->getTypeValue() != AMQP_STRING32_TYPE) {
        qDebug() << "AMQPUnwrapper::unwrapString";
        return QString();
    }
    return QString(tlv->getValue().getByteArray());
}

AMQPSymbol *AMQPUnwrapper::unwrapSymbol(TLVAMQP *tlv)
{
    if (tlv->getTypeValue() != AMQP_SYMBOL8_TYPE &&
            tlv->getTypeValue() != AMQP_SYMBOL32_TYPE) {
        qDebug() << "AMQPUnwrapper::unwrapSymbol";
        return NULL;
    }
    QString string = QString(tlv->getValue().getByteArray());
    return new AMQPSymbol(string);
}

QByteArray AMQPUnwrapper::unwrapBinary(TLVAMQP *tlv)
{
    if (tlv->getTypeValue() != AMQP_BINARY8_TYPE &&
            tlv->getTypeValue() != AMQP_BINARY32_TYPE) {
        qDebug() << "AMQPUnwrapper::unwrapData";
        return QByteArray();
    }
    return tlv->getValue().getByteArray();
}

QUuid AMQPUnwrapper::unwrapUUID(TLVAMQP *tlv)
{
    if (tlv->getTypeValue() != AMQP_UUID_TYPE) {
        qDebug() << "AMQPUnwrapper::unwrapUUID";
        return QUuid();
    }
    return QUuid(tlv->getValue().getByteArray());
}

QList<QVariant *> AMQPUnwrapper::unwrapList(TLVAMQP *tlv)
{
    if (tlv->getTypeValue() != AMQP_LIST0_TYPE &&
            tlv->getTypeValue() != AMQP_LIST8_TYPE &&
            tlv->getTypeValue() != AMQP_LIST32_TYPE) {
        qDebug() << "AMQPUnwrapper::unwrapList";
        return QList<QVariant *>();
    }
    QList<QVariant *> result = QList<QVariant *>();

    foreach (TLVAMQP *item, ((AMQPTLVList *)tlv)->getList()) {
        result.append(AMQPUnwrapper::unwrap(item));
    }
    return result;
}

QMap<QVariant *, QVariant *> AMQPUnwrapper::unwrapMap(TLVAMQP *tlv)
{
    if (tlv->getTypeValue() != AMQP_MAP8_TYPE &&
            tlv->getTypeValue() != AMQP_MAP32_TYPE) {
        qDebug() << "AMQPUnwrapper::unwrapMap";
        return QMap<QVariant *, QVariant *>();
    }

    QMap<QVariant *, QVariant *> result = QMap<QVariant *, QVariant *>();

    AMQPTLVMap *map = (AMQPTLVMap *)tlv;

    foreach (TLVAMQP *key, map->getMap().keys()) {
        TLVAMQP *value = map->getMap().value(key);
        if (value != NULL) {
            result.insert(AMQPUnwrapper::unwrap(key), AMQPUnwrapper::unwrap(value));
        }
    }
    return result;
}

QList<QVariant *> AMQPUnwrapper::unwrapArray(TLVAMQP *tlv)
{
    if (tlv->getTypeValue() != AMQP_ARRAY8_TYPE &&
            tlv->getTypeValue() != AMQP_ARRAY32_TYPE) {
        qDebug() << "AMQPUnwrapper::unwrapArray";
        return QList<QVariant *>();
    }

    QList<QVariant *> result = QList<QVariant *>();

    foreach (TLVAMQP *item, ((AMQPTLVArray *)tlv)->getElements()) {
        result.append(AMQPUnwrapper::unwrap(item));
    }
    return result;
}
