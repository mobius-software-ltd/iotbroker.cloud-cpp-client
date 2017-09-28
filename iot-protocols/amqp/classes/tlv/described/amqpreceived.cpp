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

#include "amqpreceived.h"
#include "iot-protocols/amqp/classes/headerapi/amqpunwrapper.h"
#include "iot-protocols/amqp/classes/headerapi/amqpwrapper.h"
#include "iot-protocols/amqp/classes/constructors/amqpdescribedconstructor.h"

void AMQPReceived::fill(AMQPTLVList *list)
{
    if (list->getList().count() > 0) {
        TLVAMQP *element = list->getList().at(0);
        if (!element->isNull()) {
            this->sectionNumber = new QVariant((long long)AMQPUnwrapper::unwrapUInt(element));
        }
    }

    if (list->getList().count() > 0) {
        TLVAMQP *element = list->getList().at(1);
        if (!element->isNull()) {
            this->sectionOffset = new QVariant((unsigned long long)AMQPUnwrapper::unwrapULong(element));
        }
    }
}

AMQPTLVList *AMQPReceived::getList()
{
    AMQPTLVList *list = new AMQPTLVList();

    if (this->sectionNumber != NULL) {
        list->addElementWithIndex(0, AMQPWrapper::wrapUInt(this->sectionNumber->toUInt()));
    }
    if (this->sectionOffset != NULL) {
        list->addElementWithIndex(1, AMQPWrapper::wrapULong(this->sectionOffset->toULongLong()));
    }

    ByteArray data = ByteArray();
    data.writeChar(0x23);
    AMQPType *type = new AMQPType(AMQP_SMALL_ULONG_TYPE);
    AMQPTLVFixed *fixed = new AMQPTLVFixed(type, data);

    AMQPType *constructorType = new AMQPType(list->getTypeValue());
    AMQPDescribedConstructor *constructor = new AMQPDescribedConstructor(constructorType, fixed);

    list->setConstructor(constructor);

    return list;
}

QVariant *AMQPReceived::getSectionNumber() const
{
    return sectionNumber;
}

void AMQPReceived::setSectionNumber(QVariant *value)
{
    sectionNumber = value;
}

QVariant *AMQPReceived::getSectionOffset() const
{
    return sectionOffset;
}

void AMQPReceived::setSectionOffset(QVariant *value)
{
    sectionOffset = value;
}
