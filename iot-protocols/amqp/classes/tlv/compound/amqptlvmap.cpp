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

#include "amqptlvmap.h"
#include <QDebug>

AMQPTLVMap::AMQPTLVMap() : TLVAMQP(new AMQPSimpleConstructor(new AMQPType(AMQP_MAP8_TYPE)))
{
    this->map = QMap<TLVAMQP *, TLVAMQP *>();
    this->width = 1;
    this->size = 1;
    this->count = 0;
}

AMQPTLVMap::AMQPTLVMap(AMQPType *type, QMap<TLVAMQP *, TLVAMQP *> map) : TLVAMQP(new AMQPSimpleConstructor(type))
{
    this->map = map;
    this->width = (type->getValue() == AMQP_MAP8_TYPE) ? 1 : 4;
    this->size += this->width;

    foreach (TLVAMQP *key, this->map.keys()) {
        this->size += key->getLength();
        this->size += this->map.value(key)->getLength();
    }
    this->count = this->map.count();
}

void AMQPTLVMap::putElement(TLVAMQP *key, TLVAMQP *value)
{
    this->map.insert(key, value);

    this->size += key->getLength() + value->getLength();
    this->count += 1;
    this->update();
}

QString AMQPTLVMap::description()
{
    QString string = QString();
    foreach(TLVAMQP *key, this->map.keys()) {
        string.append(key->description());
        string.append(QString(" : "));
        string.append(this->map.value(key)->description());
        string.append(QString(" \n "));
    }
    return string;
}

ByteArray AMQPTLVMap::getData()
{
    ByteArray sizeData = ByteArray();

    if (this->width == 1) {
        sizeData.writeChar(this->size);
    } else {
        sizeData.writeInt(this->size);
    }

    ByteArray countData = ByteArray();

    if (this->width == 1) {
        countData.writeChar(this->count * 2);
    } else {
        countData.writeInt(this->count * 2);
    }

    ByteArray valueData = ByteArray();

    foreach (TLVAMQP *key, this->map.keys()) {

        TLVAMQP *value = this->map.value(key);
        valueData.writeRawData(key->getData().getByteArray());
        valueData.writeRawData(value->getData().getByteArray());
    }

    ByteArray bytes = ByteArray();

    bytes.writeRawData(this->constructor->getData().getByteArray());

    if (this->size > 0) {
        bytes.writeRawData(sizeData.getByteArray());
        bytes.writeRawData(countData.getByteArray());
        bytes.writeRawData(valueData.getByteArray());
    }
    return bytes;
}

int AMQPTLVMap::getLength()
{
    return this->constructor->getLength() + this->width + this->size;
}

ByteArray AMQPTLVMap::getValue()
{
    return ByteArray();
}



int AMQPTLVMap::getWidth() const
{
    return width;
}

int AMQPTLVMap::getSize() const
{
    return size;
}

int AMQPTLVMap::getCount() const
{
    return count;
}

QMap<TLVAMQP *, TLVAMQP *> AMQPTLVMap::getMap() const
{
    return map;
}

void AMQPTLVMap::update()
{
    if (this->width == 1 && this->size > 255) {
        this->constructor->setType(new AMQPType(AMQP_MAP32_TYPE));
        this->width = 4;
        this->size + 3;
    }
}
