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

#include "amqpdisposition.h"
#include "iot-protocols/amqp/classes/constructors/amqpdescribedconstructor.h"
#include "iot-protocols/amqp/classes/tlv/fixed/amqptlvfixed.h"
#include "iot-protocols/amqp/classes/tlv/fixed/amqptlvnull.h"
#include "iot-protocols/amqp/classes/headerapi/amqpwrapper.h"
#include "iot-protocols/amqp/classes/headerapi/amqpunwrapper.h"
#include "iot-protocols/amqp/classes/headerapi/amqpfactory.h"
#include <QDebug>

AMQPDisposition::AMQPDisposition() : AMQPHeader(new AMQPHeaderCode(AMQP_DISPOSITION_HEADER_CODE))
{
    this->role = NULL;
    this->first = NULL;
    this->last = NULL;
    this->settled = NULL;
    this->state = NULL;
    this->batchable = NULL;
}

int AMQPDisposition::getLength()
{
    int length = 8;
    AMQPTLVList *arguments = this->arguments();
    length += arguments->getLength();
    return length;
}

int AMQPDisposition::getType()
{
    return AMQP_DISPOSITION_HEADER_CODE;
}

AMQPTLVList *AMQPDisposition::arguments()
{
    AMQPTLVList *list = new AMQPTLVList();

    if (this->role == NULL) {
        qDebug() << "AMQPDisposition::arguments::role";
        return new AMQPTLVList();
    }
    list->addElementWithIndex(0, AMQPWrapper::wrapBool(this->role->getValue()));

    if (this->first == NULL) {
        qDebug() << "AMQPDisposition::arguments::first";
        return new AMQPTLVList();
    }
    list->addElementWithIndex(1, AMQPWrapper::wrapUInt(AMQPSimpleType::variantToUInt(this->first)));

    if (this->last != NULL) {
        list->addElementWithIndex(2, AMQPWrapper::wrapUInt(AMQPSimpleType::variantToUInt(this->last)));
    }
    if (this->settled != NULL) {
        list->addElementWithIndex(3, AMQPWrapper::wrapBool(AMQPSimpleType::variantToBool(this->settled)));
    }
    if (this->state != NULL) {
        list->addElementWithIndex(4, this->state->getList());
    }
    if (this->batchable != NULL) {
        list->addElementWithIndex(5, AMQPWrapper::wrapBool(AMQPSimpleType::variantToBool(this->batchable)));
    }

    ByteArray data = ByteArray();
    data.writeChar(this->code->getValue());
    AMQPType *type = new AMQPType(AMQP_SMALL_ULONG_TYPE);
    AMQPTLVFixed *fixed = new AMQPTLVFixed(type, data);

    AMQPType *constructorType = new AMQPType(list->getTypeValue());
    AMQPDescribedConstructor *constructor = new AMQPDescribedConstructor(constructorType, fixed);

    list->setConstructor(constructor);
    return list;
}

void AMQPDisposition::fillArguments(AMQPTLVList *list)
{
    int size = list->getList().count();

    if (size < 2) {
        qDebug() << "AMQPDisposition::fillArguments::wrong size";
        return;
    }
    if (size > 6) {
        qDebug() << "AMQPDisposition::fillArguments::wrong size";
        return;
    }
    if (size > 0) {
        TLVAMQP *element = list->getList().at(0);
        if (element->isNull()) {
            qDebug() << "AMQPDisposition::fillArguments::role";
        }
        this->role = new AMQPRoleCode(AMQPUnwrapper::unwrapBool(element));
    }
    if (size > 1) {
        TLVAMQP *element = list->getList().at(1);
        if (element->isNull()) {
            qDebug() << "AMQPDisposition::fillArguments::role";
        }
        this->first = AMQPSimpleType::UIntToVariant(AMQPUnwrapper::unwrapUInt(element));
    }
    if (size > 2) {
        TLVAMQP *element = list->getList().at(2);
        if (!element->isNull()) {
            this->last = AMQPSimpleType::UIntToVariant(AMQPUnwrapper::unwrapUInt(element));
        }
    }
    if (size > 3) {
        TLVAMQP *element = list->getList().at(3);
        if (!element->isNull()) {
            this->settled = AMQPSimpleType::boolToVariant(AMQPUnwrapper::unwrapBool(element));
        }
    }
    if (size > 4) {
        TLVAMQP *element = list->getList().at(4);
        if (!element->isNull()) {
            if (element->getTypeValue() != AMQP_LIST0_TYPE && element->getTypeValue() != AMQP_LIST8_TYPE && element->getTypeValue() != AMQP_LIST32_TYPE) {
                qDebug() << "AMQPDisposition::fillArguments";
                return;
            }
            this->state = AMQPFactory::state((AMQPTLVList *)element);
            this->state->fill((AMQPTLVList *)element);
        }
    }
    if (size > 5) {
        TLVAMQP *element = list->getList().at(5);
        if (!element->isNull()) {
            this->batchable = AMQPSimpleType::boolToVariant(AMQPUnwrapper::unwrapBool(element));
        }
    }

}



AMQPRoleCode *AMQPDisposition::getRole() const
{
    return role;
}

void AMQPDisposition::setRole(AMQPRoleCode *value)
{
    role = value;
}

QVariant *AMQPDisposition::getFirst() const
{
    return first;
}

void AMQPDisposition::setFirst(QVariant *value)
{
    first = value;
}

QVariant *AMQPDisposition::getLast() const
{
    return last;
}

void AMQPDisposition::setLast(QVariant *value)
{
    last = value;
}

QVariant *AMQPDisposition::getSettled() const
{
    return settled;
}

void AMQPDisposition::setSettled(QVariant *value)
{
    settled = value;
}

AMQPState *AMQPDisposition::getState() const
{
    return state;
}

void AMQPDisposition::setState(AMQPState *value)
{
    state = value;
}

QVariant *AMQPDisposition::getBatchable() const
{
    return batchable;
}

void AMQPDisposition::setBatchable(QVariant *value)
{
    batchable = value;
}
