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

#include "amqpmessageheader.h"
#include "iot-protocols/amqp/classes/headerapi/amqpwrapper.h"
#include "iot-protocols/amqp/classes/headerapi/amqpunwrapper.h"
#include "iot-protocols/amqp/classes/constructors/amqpdescribedconstructor.h"

AMQPMessageHeader::AMQPMessageHeader()
{
    
}

TLVAMQP *AMQPMessageHeader::getValue()
{
    AMQPTLVList *list = new AMQPTLVList();
    
    if (this->durable != NULL) {
        list->addElementWithIndex(0, AMQPWrapper::wrapBool(AMQPSimpleType::variantToBool(this->durable)));
    }
    if (this->priority != NULL) {
        list->addElementWithIndex(1, AMQPWrapper::wrapUByte(AMQPSimpleType::variantToUByte(this->priority)));
    }
    if (this->milliseconds != NULL) {
        list->addElementWithIndex(2, AMQPWrapper::wrapUInt(AMQPSimpleType::variantToUInt(this->milliseconds)));
    }
    if (this->firstAquirer != NULL) {
        list->addElementWithIndex(3, AMQPWrapper::wrapBool(AMQPSimpleType::variantToBool(this->firstAquirer)));
    }
    if (this->deliveryCount != NULL) {
        list->addElementWithIndex(4, AMQPWrapper::wrapUInt(AMQPSimpleType::variantToUInt(this->deliveryCount)));
    }

    ByteArray data = ByteArray();
    data.writeChar(0x70);
    AMQPType *type = new AMQPType(AMQP_SMALL_ULONG_TYPE);
    AMQPTLVFixed *fixed = new AMQPTLVFixed(type, data);

    AMQPType *constructorType = new AMQPType(list->getTypeValue());
    AMQPDescribedConstructor *constructor = new AMQPDescribedConstructor(constructorType, fixed);

    list->setConstructor(constructor);
    return list;
}

AMQPSectionCode *AMQPMessageHeader::getCode()
{
    return new AMQPSectionCode(AMQP_HEADER_SECTION_CODE);
}

void AMQPMessageHeader::fill(TLVAMQP *value)
{
    AMQPTLVList *list = new AMQPTLVList();

    if (list->getList().count() > 0) {
        TLVAMQP *element = list->getList().at(0);
        if (!element->isNull()) {
            this->durable = AMQPSimpleType::boolToVariant(AMQPUnwrapper::unwrapBool(element));
        }
    }
    if (list->getList().count() > 1) {
        TLVAMQP *element = list->getList().at(1);
        if (!element->isNull()) {
            this->priority = AMQPSimpleType::UByteToVariant(AMQPUnwrapper::unwrapUByte(element));
        }
    }
    if (list->getList().count() > 2) {
        TLVAMQP *element = list->getList().at(2);
        if (!element->isNull()) {
            this->milliseconds = AMQPSimpleType::UIntToVariant(AMQPUnwrapper::unwrapUInt(element));
        }
    }
    if (list->getList().count() > 3) {
        TLVAMQP *element = list->getList().at(3);
        if (!element->isNull()) {
            this->firstAquirer = AMQPSimpleType::boolToVariant(AMQPUnwrapper::unwrapBool(element));
        }
    }
    if (list->getList().count() > 4) {
        TLVAMQP *element = list->getList().at(4);
        if (!element->isNull()) {
            this->deliveryCount = AMQPSimpleType::UIntToVariant(AMQPUnwrapper::unwrapUInt(element));
        }
    }
}

QVariant *AMQPMessageHeader::getDurable() const
{
    return durable;
}

void AMQPMessageHeader::setDurable(QVariant *value)
{
    durable = value;
}

QVariant *AMQPMessageHeader::getPriority() const
{
    return priority;
}

void AMQPMessageHeader::setPriority(QVariant *value)
{
    priority = value;
}

QVariant *AMQPMessageHeader::getMilliseconds() const
{
    return milliseconds;
}

void AMQPMessageHeader::setMilliseconds(QVariant *value)
{
    milliseconds = value;
}

QVariant *AMQPMessageHeader::getFirstAquirer() const
{
    return firstAquirer;
}

void AMQPMessageHeader::setFirstAquirer(QVariant *value)
{
    firstAquirer = value;
}

QVariant *AMQPMessageHeader::getDeliveryCount() const
{
    return deliveryCount;
}

void AMQPMessageHeader::setDeliveryCount(QVariant *value)
{
    deliveryCount = value;
}
