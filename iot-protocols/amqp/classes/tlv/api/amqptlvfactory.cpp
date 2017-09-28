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

#include "amqptlvfactory.h"
#include "iot-protocols/amqp/classes/constructors/amqpdescribedconstructor.h"
#include "iot-protocols/amqp/classes/tlv/fixed/amqptlvnull.h"
#include "iot-protocols/amqp/classes/tlv/fixed/amqptlvfixed.h"
#include "iot-protocols/amqp/classes/tlv/variable/amqptlvvariable.h"
#include "iot-protocols/amqp/classes/tlv/compound/amqptlvlist.h"
#include "iot-protocols/amqp/classes/tlv/compound/amqptlvmap.h"
#include "iot-protocols/amqp/classes/tlv/array/amqptlvarray.h"
#include <QDebug>

TLVAMQP *AMQPTLVFactory::tlvByData(ByteArray *data)
{
    AMQPSimpleConstructor *constructor = AMQPTLVFactory::constructorByData(data);
    TLVAMQP *tlv = AMQPTLVFactory::elementByConstructor(constructor, data);
    return tlv;
}

TLVAMQP *AMQPTLVFactory::elementByConstructor(AMQPSimpleConstructor *constructor, ByteArray *data)
{
    TLVAMQP *tlv = NULL;
    AMQPType *type = constructor->getType();

    qDebug() << data->readerIndex() << "::" << data->getSize() << ":" << type->getValue() << ":" << type->getName();

    switch (type->getValue()) {
        case AMQP_NULL_TYPE:
        {
            tlv = new AMQPTLVNull();
        } break;
        case AMQP_BOOLEAN_TRUE_TYPE:
        case AMQP_BOOLEAN_FALSE_TYPE:
        case AMQP_UINT0_TYPE:
        case AMQP_ULONG0_TYPE:
        {
            tlv = new AMQPTLVFixed(type, ByteArray());
        } break;
        case AMQP_BOOLEAN_TYPE:
        case AMQP_UBYTE_TYPE:
        case AMQP_BYTE_TYPE:
        case AMQP_SMALL_UINT_TYPE:
        case AMQP_SMALL_ULONG_TYPE:
        case AMQP_SMALL_INT_TYPE:
        case AMQP_SMALL_LONG_TYPE:
        {
            ByteArray bytes = ByteArray();
            bytes.writeChar(data->readChar());
            tlv = new AMQPTLVFixed(type, bytes);
        } break;
        case AMQP_SHORT_TYPE:
        case AMQP_USHORT_TYPE:
        {
            ByteArray bytes = ByteArray();
            bytes.writeShort(data->readShort());
            tlv = new AMQPTLVFixed(type, bytes);
        } break;
        case AMQP_UINT_TYPE:
        case AMQP_INT_TYPE:
        case AMQP_FLOAT_TYPE:
        case AMQP_DECIMAL32_TYPE:
        case AMQP_CHAR_TYPE:
        {
            ByteArray bytes = ByteArray();
            bytes.writeInt(data->readInt());
            tlv = new AMQPTLVFixed(type, bytes);
        } break;
        case AMQP_ULONG_TYPE:
        case AMQP_LONG_TYPE:
        case AMQP_DECIMAL64_TYPE:
        case AMQP_DOUBLE_TYPE:
        case AMQP_TIMESTAMP_TYPE:
        {
            ByteArray bytes = ByteArray();
            bytes.writeLong(data->readLong());
            tlv = new AMQPTLVFixed(type, bytes);
        } break;
        case AMQP_DECIMAL128_TYPE:
        case AMQP_UUID_TYPE:
        {
            ByteArray bytes = ByteArray(data->subFrom(0, 16));
            tlv = new AMQPTLVFixed(type, bytes);
        } break;
        case AMQP_BINARY8_TYPE:
        case AMQP_STRING8_TYPE:
        case AMQP_SYMBOL8_TYPE:
        {
            int var8Length = data->readChar() & 0xff;
            ByteArray bytes = ByteArray();
            bytes.writeRawData(data->readStringWithLength(var8Length).toUtf8());
            tlv = new AMQPTLVVariable(type, bytes);
        } break;
        case AMQP_BINARY32_TYPE:
        case AMQP_STRING32_TYPE:
        case AMQP_SYMBOL32_TYPE:
        {
            int var32Length = data->readInt();
            ByteArray bytes = ByteArray();
            bytes.writeRawData(data->readStringWithLength(var32Length).toUtf8());
            tlv = new AMQPTLVVariable(type, bytes);
        } break;
        case AMQP_LIST0_TYPE:
        {
            tlv = new AMQPTLVList();
        } break;
        case AMQP_LIST8_TYPE:
        {
            data->readChar(); // list8size
            int list8count = (data->readChar() & 0xff);
            QList<TLVAMQP *> list8values = QList<TLVAMQP *>();
            for (int i = 0; i < list8count; i++) {
                list8values.append(AMQPTLVFactory::tlvByData(data));
            }
            tlv = new AMQPTLVList(type, list8values);
        } break;
        case AMQP_LIST32_TYPE:
        {
            data->readInt(); // list32size
            int list32count = data->readInt();
            QList<TLVAMQP *> list32values = QList<TLVAMQP *>();
            for (int i = 0; i < list32count; i++) {
                list32values.append(AMQPTLVFactory::tlvByData(data));
            }
            tlv = new AMQPTLVList(type, list32values);
        } break;
        case AMQP_MAP8_TYPE:
        {
            int map8size = (data->readChar() & 0xff);
            data->readChar(); // map8count

            int stop8 = (int)(data->readerIndex() + map8size - 1);

            AMQPTLVMap *map8 = new AMQPTLVMap();

            while (data->readerIndex() < stop8) {
                map8->putElement(AMQPTLVFactory::tlvByData(data), AMQPTLVFactory::tlvByData(data));
            }
            tlv = map8;
        } break;
        case AMQP_MAP32_TYPE:
        {
            int map32size = data->readInt();
            data->readInt(); // map32count
            int stop32 = (int)(data->readerIndex() + map32size - 4);

            AMQPTLVMap *map32 = new AMQPTLVMap();

            while (data->readerIndex() < stop32) {
                map32->putElement(AMQPTLVFactory::tlvByData(data), AMQPTLVFactory::tlvByData(data));
            }
            tlv = map32;
        } break;
        case AMQP_ARRAY8_TYPE:
        {
            QList<TLVAMQP *> array8 = QList<TLVAMQP *>();
            data->readChar(); // array8size
            int array8count = (data->readChar() & 0xff);
            AMQPSimpleConstructor *array8Constructor = AMQPTLVFactory::constructorByData(data);
            for (int i = 0; i < array8count; i++) {
                array8.append(AMQPTLVFactory::elementByConstructor(array8Constructor, data));
            }
            tlv = new AMQPTLVList(type, array8);
        } break;
        case AMQP_ARRAY32_TYPE:
        {
            QList<TLVAMQP *> array32 = QList<TLVAMQP *>();
            data->readInt(); // array32size
            int array32count = (data->readInt());
            AMQPSimpleConstructor *array32Constructor = AMQPTLVFactory::constructorByData(data);
            for (int i = 0; i < array32count; i++) {
                array32.append(AMQPTLVFactory::elementByConstructor(array32Constructor, data));
            }
            tlv = new AMQPTLVList(type, array32);
        } break;
    }

    if (qobject_cast<AMQPDescribedConstructor *>(constructor) != NULL) {
        tlv->setConstructor(constructor);
    }

    return tlv;
}

AMQPSimpleConstructor *AMQPTLVFactory::constructorByData(ByteArray *data)
{
    AMQPType *type = new AMQPType(AMQP_NULL_TYPE);
    AMQPSimpleConstructor *constructor = NULL;

    int codeByte = data->readChar();

    if (codeByte == 0) {
        TLVAMQP *descriptor = AMQPTLVFactory::tlvByData(data);
        type = new AMQPType(data->readChar() & 0xff);
        constructor = new AMQPDescribedConstructor(type, descriptor);
    } else {
        type = new AMQPType(codeByte & 0xff);
        constructor = new AMQPSimpleConstructor(type);
    }

    return constructor;
}
