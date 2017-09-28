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

#include "amqpflow.h"
#include "iot-protocols/amqp/classes/constructors/amqpdescribedconstructor.h"
#include "iot-protocols/amqp/classes/headerapi/amqpwrapper.h"
#include "iot-protocols/amqp/classes/headerapi/amqpunwrapper.h"
#include "iot-protocols/amqp/classes/tlv/fixed/amqptlvfixed.h"
#include "iot-protocols/amqp/classes/tlv/fixed/amqptlvnull.h"
#include "iot-protocols/amqp/classes/amqpsimpletype.h"
#include <QDebug>

AMQPFlow::AMQPFlow() : AMQPHeader(new AMQPHeaderCode(AMQP_FLOW_HEADER_CODE))
{
    this->nextIncomingId = NULL;
    this->incomingWindow = NULL;
    this->nextOutgoingId = NULL;
    this->outgoingWindow = NULL;
    this->handle = NULL;
    this->deliveryCount = NULL;
    this->linkCredit = NULL;
    this->avaliable = NULL;
    this->drain = NULL;
    this->echo = NULL;
    this->properties = QMap<QVariant *, QVariant *>();
}

int AMQPFlow::getLength()
{
    int length = 8;
    AMQPTLVList *arguments = this->arguments();
    length += arguments->getLength();
    return length;
}

int AMQPFlow::getType()
{
    return AMQP_FLOW_HEADER_CODE;
}

AMQPTLVList *AMQPFlow::arguments()
{
    AMQPTLVList *list = new AMQPTLVList();

    if (this->nextIncomingId != NULL) {
        list->addElementWithIndex(0, AMQPWrapper::wrapUInt(AMQPSimpleType::variantToUInt(this->nextIncomingId)));
    }
    if (this->incomingWindow == NULL) {
        qDebug() << "AMQPFlow::arguments::incomingWindow";
        return new AMQPTLVList();
    }
    list->addElementWithIndex(1, AMQPWrapper::wrapUInt(AMQPSimpleType::variantToUInt(this->incomingWindow)));

    if (this->nextOutgoingId == NULL) {
        qDebug() << "AMQPFlow::arguments::nextOutgoingId";
        return new AMQPTLVList();
    }
    list->addElementWithIndex(2, AMQPWrapper::wrapUInt(AMQPSimpleType::variantToUInt(this->nextOutgoingId)));

    if (this->outgoingWindow == NULL) {
        qDebug() << "AMQPFlow::arguments::outgoingWindow";
        return new AMQPTLVList();
    }
    list->addElementWithIndex(3, AMQPWrapper::wrapUInt(AMQPSimpleType::variantToUInt(this->outgoingWindow)));

    if (this->handle != NULL) {
        list->addElementWithIndex(4, AMQPWrapper::wrapUInt(AMQPSimpleType::variantToUInt(this->handle)));
    }

    if (this->deliveryCount != NULL) {
        if (this->handle != NULL) {
            list->addElementWithIndex(5, AMQPWrapper::wrapUInt(AMQPSimpleType::variantToUInt(this->deliveryCount)));
        } else {
            qDebug() << "AMQPFlow::arguments::deliveryCount";
            return new AMQPTLVList();
        }
    }

    if (this->linkCredit != NULL) {
        if (this->handle != NULL) {
            list->addElementWithIndex(6, AMQPWrapper::wrapUInt(AMQPSimpleType::variantToUInt(this->linkCredit)));
        } else {
            qDebug() << "AMQPFlow::arguments::linkCredit";
            return new AMQPTLVList();
        }
    }

    if (this->avaliable != NULL) {
        if (this->handle != NULL) {
            list->addElementWithIndex(7, AMQPWrapper::wrapUInt(AMQPSimpleType::variantToUInt(this->avaliable)));
        } else {
            qDebug() << "AMQPFlow::arguments::avaliable";
            return new AMQPTLVList();
        }
    }

    if (this->drain != NULL) {
        if (this->handle != NULL) {
            list->addElementWithIndex(8, AMQPWrapper::wrapBool(AMQPSimpleType::variantToBool(this->drain)));
        } else {
            qDebug() << "AMQPFlow::arguments::drain";
            return new AMQPTLVList();
        }
    }

    if (this->echo != NULL) {
        list->addElementWithIndex(9, AMQPWrapper::wrapBool(AMQPSimpleType::variantToBool(this->echo)));
    }

    if (this->properties.count() != 0) {
        list->addElementWithIndex(10, AMQPWrapper::wrapMap(this->properties));
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

void AMQPFlow::fillArguments(AMQPTLVList *list)
{
    int size = list->getList().count();

    if (size < 4) {
        qDebug() << "AMQPFlow::arguments::wrong_size";
    }
    if (size > 11) {
        qDebug() << "AMQPFlow::arguments::wrong_size";
    }

    if (size > 0) {
        TLVAMQP *element = list->getList().at(0);
        if (!element->isNull()) {
            this->nextIncomingId = AMQPSimpleType::UIntToVariant(AMQPUnwrapper::unwrapUInt(element));
        }
    }
    if (size > 1) {
        TLVAMQP *element = list->getList().at(1);
        if (element->isNull()) {
            qDebug() << "AMQPFlow::arguments::incomingWindow";
        }
        this->incomingWindow = AMQPSimpleType::UIntToVariant(AMQPUnwrapper::unwrapUInt(element));
    }
    if (size > 2) {
        TLVAMQP *element = list->getList().at(2);
        if (element->isNull()) {
            qDebug() << "AMQPFlow::arguments::nextOutgoingId";
        }
        this->nextOutgoingId = AMQPSimpleType::UIntToVariant(AMQPUnwrapper::unwrapUInt(element));
    }
    if (size > 3) {
        TLVAMQP *element = list->getList().at(3);
        if (element->isNull()) {
            qDebug() << "AMQPFlow::arguments::outgoingWindow";
        }
        this->outgoingWindow = AMQPSimpleType::UIntToVariant(AMQPUnwrapper::unwrapUInt(element));
    }
    if (size > 4) {
        TLVAMQP *element = list->getList().at(4);
        if (!element->isNull()) {
            this->handle = AMQPSimpleType::UIntToVariant(AMQPUnwrapper::unwrapUInt(element));
        }
    }
    if (size > 5) {
        TLVAMQP *element = list->getList().at(5);
        if (!element->isNull()) {
            if (this->handle != NULL) {
                this->deliveryCount = AMQPSimpleType::UIntToVariant(AMQPUnwrapper::unwrapUInt(element));
            } else {
                qDebug() << "AMQPFlow::arguments::deliveryCount";
            }
        }
    }
    if (size > 6) {
        TLVAMQP *element = list->getList().at(6);
        if (!element->isNull()) {
            if (this->handle != NULL) {
                this->linkCredit = AMQPSimpleType::UIntToVariant(AMQPUnwrapper::unwrapUInt(element));
            } else {
                qDebug() << "AMQPFlow::arguments::linkCredit";
            }
        }
    }
    if (size > 7) {
        TLVAMQP *element = list->getList().at(7);
        if (!element->isNull()) {
            if (this->handle != NULL) {
                this->avaliable = AMQPSimpleType::UIntToVariant(AMQPUnwrapper::unwrapUInt(element));
            } else {
                qDebug() << "AMQPFlow::arguments::avaliable";
            }
        }
    }
    if (size > 8) {
        TLVAMQP *element = list->getList().at(8);
        if (!element->isNull()) {
            if (this->handle != NULL) {
                this->drain = AMQPSimpleType::boolToVariant(AMQPUnwrapper::unwrapBool(element));
            } else {
                qDebug() << "AMQPFlow::arguments::drain";
            }
        }
    }
    if (size > 9) {
        TLVAMQP *element = list->getList().at(9);
        if (!element->isNull()) {
            this->echo = AMQPSimpleType::boolToVariant(AMQPUnwrapper::unwrapBool(element));
        }
    }
    if (size > 10) {
        TLVAMQP *element = list->getList().at(10);
        if (!element->isNull()) {
            this->properties = AMQPUnwrapper::unwrapMap(element);
        }
    }

}

void AMQPFlow::addProperty(QString key, QVariant *value)
{
    QVariant *variantKey = AMQPSimpleType::symbolToVariant(new AMQPSymbol(key));
    this->properties.insert(variantKey, value);
}

QVariant *AMQPFlow::getNextIncomingId() const
{
    return nextIncomingId;
}

void AMQPFlow::setNextIncomingId(QVariant *value)
{
    nextIncomingId = value;
}

QVariant *AMQPFlow::getIncomingWindow() const
{
    return incomingWindow;
}

void AMQPFlow::setIncomingWindow(QVariant *value)
{
    incomingWindow = value;
}

QVariant *AMQPFlow::getNextOutgoingId() const
{
    return nextOutgoingId;
}

void AMQPFlow::setNextOutgoingId(QVariant *value)
{
    nextOutgoingId = value;
}

QVariant *AMQPFlow::getOutgoingWindow() const
{
    return outgoingWindow;
}

void AMQPFlow::setOutgoingWindow(QVariant *value)
{
    outgoingWindow = value;
}

QVariant *AMQPFlow::getHandle() const
{
    return handle;
}

void AMQPFlow::setHandle(QVariant *value)
{
    handle = value;
}

QVariant *AMQPFlow::getDeliveryCount() const
{
    return deliveryCount;
}

void AMQPFlow::setDeliveryCount(QVariant *value)
{
    deliveryCount = value;
}

QVariant *AMQPFlow::getLinkCredit() const
{
    return linkCredit;
}

void AMQPFlow::setLinkCredit(QVariant *value)
{
    linkCredit = value;
}

QVariant *AMQPFlow::getAvaliable() const
{
    return avaliable;
}

void AMQPFlow::setAvaliable(QVariant *value)
{
    avaliable = value;
}

QVariant *AMQPFlow::getDrain() const
{
    return drain;
}

void AMQPFlow::setDrain(QVariant *value)
{
    drain = value;
}

QVariant *AMQPFlow::getEcho() const
{
    return echo;
}

void AMQPFlow::setEcho(QVariant *value)
{
    echo = value;
}

QMap<QVariant *, QVariant *> AMQPFlow::getProperties() const
{
    return properties;
}

void AMQPFlow::setProperties(const QMap<QVariant *, QVariant *> &value)
{
    properties = value;
}
