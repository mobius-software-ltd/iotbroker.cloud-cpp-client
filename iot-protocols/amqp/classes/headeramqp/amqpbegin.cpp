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

#include "amqpbegin.h"
#include "iot-protocols/amqp/classes/constructors/amqpdescribedconstructor.h"
#include "iot-protocols/amqp/classes/headerapi/amqpwrapper.h"
#include "iot-protocols/amqp/classes/headerapi/amqpunwrapper.h"
#include "iot-protocols/amqp/classes/tlv/fixed/amqptlvfixed.h"
#include "iot-protocols/amqp/classes/tlv/fixed/amqptlvnull.h"
#include "iot-protocols/amqp/classes/amqpsimpletype.h"
#include <QDebug>

AMQPBegin::AMQPBegin() : AMQPHeader(new AMQPHeaderCode(AMQP_BEGIN_HEADER_CODE))
{
    this->remoteChannel = NULL;
    this->nextOutgoingID = NULL;
    this->incomingWindow = NULL;
    this->outgoingWindow = NULL;
    this->handleMax = NULL;
    this->offeredCapabilities = QList<QVariant *>();
    this->desiredCapabilities = QList<QVariant *>();
    this->properties = QMap<QVariant *, QVariant *>();
}

int AMQPBegin::getLength()
{
    int length = 8;
    AMQPTLVList *arguments = this->arguments();
    length += arguments->getLength();
    return length;
}

int AMQPBegin::getType()
{
    return AMQP_BEGIN_HEADER_CODE;
}

AMQPTLVList *AMQPBegin::arguments()
{
    AMQPTLVList *list = new AMQPTLVList();

    if (this->remoteChannel != NULL) {
        list->addElementWithIndex(0, AMQPWrapper::wrapUShort(AMQPSimpleType::variantToUShort(this->remoteChannel)));
    }
    if (this->nextOutgoingID == NULL) {
        qDebug() << "AMQPBegin::arguments::nextOutgoingID";
        return new AMQPTLVList();
    }
    list->addElementWithIndex(1, AMQPWrapper::wrapUInt(AMQPSimpleType::variantToUInt(this->nextOutgoingID)));

    if (this->incomingWindow == NULL) {
        qDebug() << "AMQPBegin::arguments::incomingWindow";
        return new AMQPTLVList();
    }
    list->addElementWithIndex(2, AMQPWrapper::wrapUInt(AMQPSimpleType::variantToUInt(this->incomingWindow)));

    if (this->outgoingWindow == NULL) {
        qDebug() << "AMQPBegin::arguments::outgoingWindow";
        return new AMQPTLVList();
    }
    list->addElementWithIndex(3, AMQPWrapper::wrapUInt(AMQPSimpleType::variantToUInt(this->outgoingWindow)));

    if (this->handleMax != NULL) {
        list->addElementWithIndex(4, AMQPWrapper::wrapUInt(AMQPSimpleType::variantToUInt(this->handleMax)));
    }
    if (this->offeredCapabilities.count() != 0) {
        list->addElementWithIndex(5, AMQPWrapper::wrapArray(this->offeredCapabilities));
    }
    if (this->desiredCapabilities.count() != 0) {
        list->addElementWithIndex(6, AMQPWrapper::wrapArray(this->desiredCapabilities));
    }
    if (this->properties.count() != 0) {
        list->addElementWithIndex(7, AMQPWrapper::wrapMap(this->properties));
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

void AMQPBegin::fillArguments(AMQPTLVList *list)
{
    int size = list->getList().count();

    if (size < 4) {
        qDebug() << "AMQPBegin::fillArguments::size == 0";
        return;
    }
    if (size > 8) {
        qDebug() << "AMQPBegin::fillArguments::wrong_size";
        return;
    }
    if (size > 0) {
        TLVAMQP *element = list->getList().at(0);
        if (!element->isNull()) {
            this->remoteChannel = AMQPSimpleType::UShortToVariant(AMQPUnwrapper::unwrapUShort(element));
        }
    }
    if (size > 1) {
        TLVAMQP *element = list->getList().at(1);
        if (element->isNull()) {
            qDebug() << "AMQPBegin::fillArguments::nextOutgoingID";
        }
        this->nextOutgoingID = AMQPSimpleType::UIntToVariant(AMQPUnwrapper::unwrapUInt(element));
    }
    if (size > 2) {
        TLVAMQP *element = list->getList().at(2);
        if (element->isNull()) {
            qDebug() << "AMQPBegin::fillArguments::incomingWindow";
        }
        this->incomingWindow = AMQPSimpleType::UIntToVariant(AMQPUnwrapper::unwrapUInt(element));
    }
    if (size > 3) {
        TLVAMQP *element = list->getList().at(3);
        if (element->isNull()) {
            qDebug() << "AMQPBegin::fillArguments::outgoingWindow";
        }
        this->outgoingWindow = AMQPSimpleType::UIntToVariant(AMQPUnwrapper::unwrapUInt(element));
    }
    if (size > 4) {
        TLVAMQP *element = list->getList().at(4);
        if (!element->isNull()) {
            this->handleMax = AMQPSimpleType::UIntToVariant(AMQPUnwrapper::unwrapUInt(element));
        }
    }
    if (size > 5) {
        TLVAMQP *element = list->getList().at(5);
        if (!element->isNull()) {
            this->offeredCapabilities = AMQPUnwrapper::unwrapArray(element);
        }
    }
    if (size > 6) {
        TLVAMQP *element = list->getList().at(6);
        if (!element->isNull()) {
            this->desiredCapabilities = AMQPUnwrapper::unwrapArray(element);
        }
    }
    if (size > 7) {
        TLVAMQP *element = list->getList().at(7);
        if (!element->isNull()) {
            this->properties = AMQPUnwrapper::unwrapMap(element);
        }
    }
}



void AMQPBegin::addOfferedCapability(QList<QString> list)
{
    foreach (QString item, list) {
        QVariant *variant = AMQPSimpleType::symbolToVariant(new AMQPSymbol(item));
        this->offeredCapabilities.append(variant);
    }
}

void AMQPBegin::addDesiredCapability(QList<QString> list)
{
    foreach (QString item, list) {
        QVariant *variant = AMQPSimpleType::symbolToVariant(new AMQPSymbol(item));
        this->desiredCapabilities.append(variant);
    }
}

void AMQPBegin::addProperty(QString key, QVariant *value)
{
    QVariant *variantKey = AMQPSimpleType::symbolToVariant(new AMQPSymbol(key));
    this->properties.insert(variantKey, value);
}



QVariant *AMQPBegin::getRemoteChannel() const
{
    return remoteChannel;
}

void AMQPBegin::setRemoteChannel(QVariant *value)
{
    remoteChannel = value;
}

QVariant *AMQPBegin::getNextOutgoingID() const
{
    return nextOutgoingID;
}

void AMQPBegin::setNextOutgoingID(QVariant *value)
{
    nextOutgoingID = value;
}

QVariant *AMQPBegin::getIncomingWindow() const
{
    return incomingWindow;
}

void AMQPBegin::setIncomingWindow(QVariant *value)
{
    incomingWindow = value;
}

QVariant *AMQPBegin::getOutgoingWindow() const
{
    return outgoingWindow;
}

void AMQPBegin::setOutgoingWindow(QVariant *value)
{
    outgoingWindow = value;
}

QVariant *AMQPBegin::getHandleMax() const
{
    return handleMax;
}

void AMQPBegin::setHandleMax(QVariant *value)
{
    handleMax = value;
}

QList<QVariant *> AMQPBegin::getOfferedCapabilities() const
{
    return offeredCapabilities;
}

void AMQPBegin::setOfferedCapabilities(const QList<QVariant *> &value)
{
    offeredCapabilities = value;
}

QList<QVariant *> AMQPBegin::getDesiredCapabilities() const
{
    return desiredCapabilities;
}

void AMQPBegin::setDesiredCapabilities(const QList<QVariant *> &value)
{
    desiredCapabilities = value;
}

QMap<QVariant *, QVariant *> AMQPBegin::getProperties() const
{
    return properties;
}

void AMQPBegin::setProperties(const QMap<QVariant *, QVariant *> &value)
{
    properties = value;
}
