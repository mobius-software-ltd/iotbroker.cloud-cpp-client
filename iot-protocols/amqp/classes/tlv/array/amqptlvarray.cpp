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

#include "amqptlvarray.h"
#include <QDebug>

AMQPTLVArray::AMQPTLVArray() : TLVAMQP(new AMQPSimpleConstructor(new AMQPType(AMQP_ARRAY8_TYPE)))
{
    this->elements = QList<TLVAMQP *>();
    this->width = 1;
    this->count = 0;
    this->size = 0;
}

AMQPTLVArray::AMQPTLVArray(AMQPType *type, QList<TLVAMQP *>elements) : TLVAMQP(new AMQPSimpleConstructor(type))
{
    this->elements = elements;
    this->width = type->getValue() == AMQP_ARRAY8_TYPE ? 1 : 4;
    this->size += this->width;

    foreach (TLVAMQP *element, this->elements) {
        this->size += element->getLength() - element->getConstructor()->getLength();
        if (this->elementContructor == NULL && element != NULL) {
            this->elementContructor = element->getConstructor();
        }
    }
    this->size = this->elementContructor->getLength();
    this->count = this->elements.count();
}

void AMQPTLVArray::addElement(TLVAMQP *element)
{
    if (this->elements.count() == 0) {
        this->elementContructor = element->getConstructor();
        this->size += this->width;
        this->size += this->elementContructor->getLength();
    }
    this->elements.append(element);
    this->count += 1;
    this->size += element->getLength() - this->elementContructor->getLength();
    if (this->width == 1 && this->size > 255) {
        this->constructor->setType(new AMQPType(AMQP_ARRAY32_TYPE));
        this->width = 4;
        this->size += 3;
    }
}

ByteArray AMQPTLVArray::getData()
{
    ByteArray sizeData = ByteArray();

    if (this->width == 1) {
        sizeData.writeChar(this->size);
    } else if (this->width == 4) {
        sizeData.writeInt(this->size);
    }

    ByteArray countData = ByteArray();

    if (this->width == 1) {
        countData.writeChar(this->count);
    } else if (this->width == 4) {
        countData.writeInt(this->count);
    }

    ByteArray elementConstructorData = this->elementContructor->getData();
    ByteArray valueData = ByteArray();
    ByteArray tlvData = ByteArray();

    foreach (TLVAMQP *item, this->elements) {
        int valueLength = item->getData().getSize() - elementConstructorData.getSize();
        ByteArray sub = item->getData().subFrom(elementConstructorData.getSize(), valueLength);
        tlvData = sub;
        valueData.writeRawData(tlvData.getByteArray());
    }

    ByteArray bytes = ByteArray();

    bytes.writeRawData(this->constructor->getData().getByteArray());

    if (this->size > 0) {
        bytes.writeRawData(sizeData.getByteArray());
        bytes.writeRawData(countData.getByteArray());
        bytes.writeRawData(elementConstructorData.getByteArray());
        bytes.writeRawData(valueData.getByteArray());
    }

    return bytes;
}

QString AMQPTLVArray::description()
{
    QString string = QString();

    foreach (TLVAMQP *item, this->elements) {
        string.append(item->description());
        string.append("\n");
    }

    return string;
}

int AMQPTLVArray::getLength()
{
    return this->constructor->getLength() + this->size + this->width;
}

ByteArray AMQPTLVArray::getValue()
{
    return ByteArray();
}

bool AMQPTLVArray::isNull()
{
    AMQPType *type = this->constructor->getType();
    if (type->getValue() == AMQP_NULL_TYPE) {
        return true;
    }
    if (type->getValue() == AMQP_ARRAY8_TYPE || type->getValue() == AMQP_ARRAY32_TYPE) {
        if (this->elements.count() == 0) {
            return true;
        }
    }
    return false;
}



int AMQPTLVArray::getWidth() const
{
    return width;
}

int AMQPTLVArray::getSize() const
{
    return size;
}

int AMQPTLVArray::getCount() const
{
    return count;
}

QList<TLVAMQP *> AMQPTLVArray::getElements() const
{
    return elements;
}

AMQPSimpleConstructor *AMQPTLVArray::getElementContructor() const
{
    return elementContructor;
}
