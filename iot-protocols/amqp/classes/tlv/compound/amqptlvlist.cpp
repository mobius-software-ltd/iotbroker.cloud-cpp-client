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

#include "amqptlvlist.h"
#include "iot-protocols/amqp/classes/tlv/fixed/amqptlvnull.h"
#include "iot-protocols/amqp/classes/tlv/compound/amqptlvmap.h"
#include "iot-protocols/amqp/classes/tlv/array/amqptlvarray.h"
#include <QDebug>

AMQPTLVList::AMQPTLVList() : TLVAMQP(new AMQPSimpleConstructor(new AMQPType(AMQP_LIST0_TYPE)))
{
    this->list = QList<TLVAMQP *>();
    this->width = 0;
    this->count = 0;
    this->size = 0;
}

AMQPTLVList::AMQPTLVList(AMQPType *type, QList<TLVAMQP *> value) : TLVAMQP(new AMQPSimpleConstructor(type))
{
    this->list = value;
    this->width = type->getValue() == AMQP_LIST8_TYPE ? 1 : 4;
    this->size += this->width;
    foreach (TLVAMQP *item, this->list) {
        this->size += item->getLength();
    }
    this->count = this->list.count();
}

void AMQPTLVList::addElement(TLVAMQP *element)
{
    if (this->size == 0) {
        this->constructor->setType(new AMQPType(AMQP_LIST8_TYPE));
        this->width = 1;
        this->size += 1;
    }
    this->list.append(element);
    this->count += 1;
    this->size += element->getLength();
    this->update();
}

void AMQPTLVList::setElementWithIndex(int index, TLVAMQP *element)
{
    this->size -= this->list.at(index)->getLength();
    this->list.replace(index, element);
    this->size += element->getLength();
    this->update();
}

void AMQPTLVList::addElementWithIndex(int index, TLVAMQP *element)
{
    int diff = (int)(index - this->list.count());
    do {
        this->addElement(new AMQPTLVNull());
    } while (diff-- > 0);
    this->setElementWithIndex(index, element);
}

void AMQPTLVList::addToListWithIndex(int index, int elementIndex, TLVAMQP *element)
{
    if (this->count <= index) {
        this->addElementWithIndex(index, new AMQPTLVList());
    }
    TLVAMQP *list = this->list.at(index);
    if (list->isNull()) {
        this->setElementWithIndex(index, new AMQPTLVList());
    }
    ((AMQPTLVList *)this->list.at(index))->addElementWithIndex(elementIndex, element);
    this->size += element->getLength();
    this->update();
}

void AMQPTLVList::addToMapWithIndex(int index, TLVAMQP *key, TLVAMQP *value)
{
    if (this->count <= index) {
        this->addElementWithIndex(index, new AMQPTLVMap());
    }
    TLVAMQP *map = this->list.at(index);
    if (map->isNull()) {
        this->setElementWithIndex(index, new AMQPTLVMap());
    }
    ((AMQPTLVMap *)this->list.at(index))->putElement(key, value);
    this->size += key->getLength() + value->getLength();
    this->update();
}

void AMQPTLVList::addToArrayWithIndex(int index, TLVAMQP *element)
{
    if (this->count <= index) {
        this->addElementWithIndex(index, new AMQPTLVArray());
    }
    TLVAMQP *array = this->list.at(index);
    if (array->isNull()) {
        this->setElementWithIndex(index, new AMQPTLVArray());
    }
    ((AMQPTLVArray *)this->list.at(index))->addElement(element);
    this->size += element->getLength();
    this->update();
}



ByteArray AMQPTLVList::getData()
{
    ByteArray sizeData = ByteArray();

    if (this->width == 1) {
        sizeData.writeChar(this->size);
    } else {
        sizeData.writeInt(this->size);
    }

    ByteArray countBytes = ByteArray();

    if (this->width == 1) {
        countBytes.writeChar(this->count);
    } else {
        countBytes.writeInt(this->count);
    }

    ByteArray valueData = ByteArray();

    foreach (TLVAMQP *item, this->list) {
        valueData.writeRawData(item->getData().getByteArray());
    }

    ByteArray bytes = ByteArray();

    bytes.writeRawData(this->constructor->getData().getByteArray());

    if (this->size > 0) {
        bytes.writeRawData(sizeData.getByteArray());
        bytes.writeRawData(countBytes.getByteArray());
        bytes.writeRawData(valueData.getByteArray());
    }

    return bytes;
}

int AMQPTLVList::getLength()
{
    return this->constructor->getLength() + this->width + this->size;
}

ByteArray AMQPTLVList::getValue()
{
    return ByteArray();
}

QString AMQPTLVList::description()
{
    QString string = QString();
    foreach(TLVAMQP *item, this->list) {
        string.append(item->description());
        string.append(QString("\n"));
    }
    return string;
}



int AMQPTLVList::getWidth() const
{
    return width;
}

int AMQPTLVList::getSize() const
{
    return size;
}

int AMQPTLVList::getCount() const
{
    return count;
}

QList<TLVAMQP *> AMQPTLVList::getList() const
{
    return list;
}

void AMQPTLVList::update()
{
    if (this->width == 1 && this->size > 255) {
        this->constructor->setType(new AMQPType(AMQP_LIST32_TYPE));
        this->width = 4;
        this->size += 3;
    }
}
