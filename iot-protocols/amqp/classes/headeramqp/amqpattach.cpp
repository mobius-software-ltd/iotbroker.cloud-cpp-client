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

#include "amqpattach.h"
#include "iot-protocols/amqp/classes/amqpsimpletype.h"
#include "iot-protocols/amqp/classes/headerapi/amqpwrapper.h"
#include "iot-protocols/amqp/classes/headerapi/amqpunwrapper.h"
#include "iot-protocols/amqp/classes/constructors/amqpdescribedconstructor.h"
#include <QDebug>

AMQPAttach::AMQPAttach() : AMQPHeader(new AMQPHeaderCode(AMQP_ATTACH_HEADER_CODE))
{
    this->handle = NULL;
    this->role = NULL;
    this->sendCodes = NULL;
    this->receivedCodes = NULL;
    this->source = NULL;
    this->target = NULL;
    this->incompleteUnsettled = NULL;
    this->initialDeliveryCount = NULL;
    this->maxMessageSize = NULL;
    this->unsettled = QMap<QVariant *, QVariant *>();
    this->offeredCapabilities = QList<QVariant *>();
    this->desiredCapabilities = QList<QVariant *>();
    this->properties = QMap<QVariant *, QVariant *>();
}

int AMQPAttach::getLength()
{
    int length = 8;
    AMQPTLVList *arguments = this->arguments();
    length += arguments->getLength();

    return length;
}

int AMQPAttach::getType()
{
    return AMQP_ATTACH_HEADER_CODE;
}

AMQPTLVList *AMQPAttach::arguments()
{
    AMQPTLVList *list = new AMQPTLVList();

    if (this->name.isNull()) {
        qDebug() << "AMQPAttach::arguments::name";
        return new AMQPTLVList();
    }
    list->addElementWithIndex(0, AMQPWrapper::wrapString(this->name));

    if (this->handle == NULL) {
        qDebug() << "AMQPAttach::arguments::handle";
        return new AMQPTLVList();
    }
    list->addElementWithIndex(1, AMQPWrapper::wrapUInt(AMQPSimpleType::variantToUInt(this->handle)));

    if (this->role == NULL) {
        qDebug() << "AMQPAttach::arguments::role";
        return new AMQPTLVList();
    }
    list->addElementWithIndex(2, AMQPWrapper::wrapBool(this->role->getValue()));
    qDebug() << " ---01";

    if (this->sendCodes != NULL) {
        list->addElementWithIndex(3, AMQPWrapper::wrapUByte(this->sendCodes->getValue()));
    }    qDebug() << " ---02";

    if (this->receivedCodes != NULL) {
        list->addElementWithIndex(4, AMQPWrapper::wrapUByte(this->receivedCodes->getValue()));
    }    qDebug() << " ---03";

    if (this->source != NULL) {
        list->addElementWithIndex(5, this->source->getList());
    }    qDebug() << " ---04";

    if (this->target != NULL) {
        list->addElementWithIndex(6, this->target->getlist());
    }    qDebug() << " ---05";

    if (this->unsettled.count() != 0) {
        list->addElementWithIndex(7, AMQPWrapper::wrapMap(this->unsettled));
    }    qDebug() << " ---06";

    if (this->incompleteUnsettled != NULL) {
        list->addElementWithIndex(8, AMQPWrapper::wrapBool(AMQPSimpleType::variantToBool(this->incompleteUnsettled)));
    }    qDebug() << " ---07";

    if (this->initialDeliveryCount != NULL) {
        list->addElementWithIndex(9, AMQPWrapper::wrapUInt(AMQPSimpleType::variantToUInt(this->initialDeliveryCount)));
    } else if (this->role->getValue() == AMQP_SENDER_ROLE_CODE) {
        qDebug() << "AMQPAttach::arguments::initialDeliveryCount";
        return new AMQPTLVList();
    }    qDebug() << " ---08";

    if (this->maxMessageSize != NULL) {
        list->addElementWithIndex(10, AMQPWrapper::wrapULong(AMQPSimpleType::variantToULong(this->maxMessageSize)));
    }    qDebug() << " ---09";

    if (this->offeredCapabilities.count() != 0) {
        list->addElementWithIndex(11, AMQPWrapper::wrapArray(this->offeredCapabilities));
    }    qDebug() << " ---01";

    if (this->desiredCapabilities.count() != 0) {
        list->addElementWithIndex(12, AMQPWrapper::wrapArray(this->desiredCapabilities));
    }    qDebug() << " ---01";

    if (this->properties.count() != 0) {
        list->addElementWithIndex(13, AMQPWrapper::wrapMap(this->properties));
    }    qDebug() << " ---01";


    ByteArray data = ByteArray();
    data.writeChar(this->code->getValue());
    AMQPType *type = new AMQPType(AMQP_SMALL_ULONG_TYPE);
    AMQPTLVFixed *fixed = new AMQPTLVFixed(type, data);

    AMQPType *constructorType = new AMQPType(list->getTypeValue());
    AMQPDescribedConstructor *constructor = new AMQPDescribedConstructor(constructorType, fixed);

    list->setConstructor(constructor);
    return list;
}

void AMQPAttach::fillArguments(AMQPTLVList *list)
{
    int size = list->getList().count();

    if (size < 3) {
        qDebug() << "AMQPAttach::fillArguments";
        return;
    }
    if (size > 14) {
        qDebug() << "AMQPAttach::fillArguments";
        return;
    }
    if (size > 0) {
        TLVAMQP *element = list->getList().at(0);
        if (element->isNull()) {
            qDebug() << "AMQPAttach::fillArguments::name";
            return;
        }
        this->name = AMQPUnwrapper::unwrapString(element);
    }
    if (size > 1) {
        TLVAMQP *element = list->getList().at(1);
        if (element->isNull()) {
            qDebug() << "AMQPAttach::fillArguments::handle";
            return;
        }
        this->handle = AMQPSimpleType::UIntToVariant(AMQPUnwrapper::unwrapUInt(element));
    }
    if (size > 2) {
        TLVAMQP *element = list->getList().at(2);
        if (element->isNull()) {
            qDebug() << "AMQPAttach::fillArguments::role";
            return;
        }
        this->role = new AMQPRoleCode(AMQPUnwrapper::unwrapBool(element));
    }
    if (size > 3) {
        TLVAMQP *element = list->getList().at(3);
        if (!element->isNull()) {
            this->sendCodes = new AMQPSendCode(AMQPUnwrapper::unwrapUByte(element));
        }
    }
    if (size > 4) {
        TLVAMQP *element = list->getList().at(4);
        if (!element->isNull()) {
            this->receivedCodes = new AMQPReceiverSettleMode(AMQPUnwrapper::unwrapUByte(element));
        }
    }
    if (size > 5) {
        TLVAMQP *element = list->getList().at(5);
        if (!element->isNull()) {
            if (element->getTypeValue() != AMQP_LIST0_TYPE && element->getTypeValue() != AMQP_LIST8_TYPE && element->getTypeValue() != AMQP_LIST32_TYPE) {
                qDebug() << "AMQPAttach::fillArguments::source";
                return;
            }
            this->source = new AMQPSource();
            this->source->fill((AMQPTLVList *)element);
        }
    }
    if (size > 6) {
        TLVAMQP *element = list->getList().at(6);
        if (!element->isNull()) {
            if (element->getTypeValue() != AMQP_LIST0_TYPE && element->getTypeValue() != AMQP_LIST8_TYPE && element->getTypeValue() != AMQP_LIST32_TYPE) {
                qDebug() << "AMQPAttach::fillArguments::target";
                return;
            }
            this->target = new AMQPTarget();
            this->target->fill((AMQPTLVList *)element);
        }
    }
    if (size > 7) {
        TLVAMQP *element = list->getList().at(7);
        if (!element->isNull()) {
            this->unsettled = AMQPUnwrapper::unwrapMap(element);
        }
    }
    if (size > 8) {
        TLVAMQP *element = list->getList().at(8);
        if (!element->isNull()) {
            this->incompleteUnsettled = AMQPSimpleType::boolToVariant(AMQPUnwrapper::unwrapBool(element));
        }
    }
    if (size > 9) {
        TLVAMQP *element = list->getList().at(9);
        if (!element->isNull()) {
            this->initialDeliveryCount = AMQPSimpleType::UIntToVariant(AMQPUnwrapper::unwrapUInt(element));
        } else {
            qDebug() << "AMQPAttach::fillArguments::initialDeliveryCount";
            return;
        }
    }
    if (size > 10) {
        TLVAMQP *element = list->getList().at(10);
        if (!element->isNull()) {
            this->maxMessageSize = AMQPSimpleType::ULongToVariant(AMQPUnwrapper::unwrapULong(element));
        }
    }
    if (size > 11) {
        TLVAMQP *element = list->getList().at(11);
        if (!element->isNull()) {
            this->offeredCapabilities = AMQPUnwrapper::unwrapArray(element);
        }
    }
    if (size > 12) {
        TLVAMQP *element = list->getList().at(12);
        if (!element->isNull()) {
            this->desiredCapabilities = AMQPUnwrapper::unwrapArray(element);
        }
    }
    if (size > 13) {
        TLVAMQP *element = list->getList().at(13);
        if (!element->isNull()) {
            this->properties = AMQPUnwrapper::unwrapMap(element);
        }
    }
}

void AMQPAttach::addUnsettled(QString key, QVariant *value)
{
    QVariant *keyVariant = AMQPSimpleType::symbolToVariant(new AMQPSymbol(key));
    this->unsettled.insert(keyVariant, value);
}

void AMQPAttach::addOfferedCapability(QList<QString> array)
{
    foreach (QString item, array) {
        QVariant *itemVariant = AMQPSimpleType::symbolToVariant(new AMQPSymbol(item));
        this->offeredCapabilities.append(itemVariant);
    }
}

void AMQPAttach::addDesiredCapability(QList<QString> array)
{
    foreach (QString item, array) {
        QVariant *itemVariant = AMQPSimpleType::symbolToVariant(new AMQPSymbol(item));
        this->desiredCapabilities.append(itemVariant);
    }
}

void AMQPAttach::addProperty(QString key, QVariant *value)
{
    QVariant *keyVariant = AMQPSimpleType::symbolToVariant(new AMQPSymbol(key));
    this->properties.insert(keyVariant, value);
}



QString AMQPAttach::getName() const
{
    return name;
}

void AMQPAttach::setName(const QString &value)
{
    name = value;
}

QVariant *AMQPAttach::getHandle() const
{
    return handle;
}

void AMQPAttach::setHandle(QVariant *value)
{
    handle = value;
}

AMQPRoleCode *AMQPAttach::getRole() const
{
    return role;
}

void AMQPAttach::setRole(AMQPRoleCode *value)
{
    role = value;
}

AMQPSendCode *AMQPAttach::getSendCodes() const
{
    return sendCodes;
}

void AMQPAttach::setSendCodes(AMQPSendCode *value)
{
    sendCodes = value;
}

AMQPReceiverSettleMode *AMQPAttach::getReceivedCodes() const
{
    return receivedCodes;
}

void AMQPAttach::setReceivedCodes(AMQPReceiverSettleMode *value)
{
    receivedCodes = value;
}

AMQPSource *AMQPAttach::getSource() const
{
    return source;
}

void AMQPAttach::setSource(AMQPSource *value)
{
    source = value;
}

AMQPTarget *AMQPAttach::getTarget() const
{
    return target;
}

void AMQPAttach::setTarget(AMQPTarget *value)
{
    target = value;
}

QMap<QVariant *, QVariant *> AMQPAttach::getUnsettled() const
{
    return unsettled;
}

void AMQPAttach::setUnsettled(const QMap<QVariant *, QVariant *> &value)
{
    unsettled = value;
}

QVariant *AMQPAttach::getIncompleteUnsettled() const
{
    return incompleteUnsettled;
}

void AMQPAttach::setIncompleteUnsettled(QVariant *value)
{
    incompleteUnsettled = value;
}

QVariant *AMQPAttach::getInitialDeliveryCount() const
{
    return initialDeliveryCount;
}

void AMQPAttach::setInitialDeliveryCount(QVariant *value)
{
    initialDeliveryCount = value;
}

QVariant *AMQPAttach::getMaxMessageSize() const
{
    return maxMessageSize;
}

void AMQPAttach::setMaxMessageSize(QVariant *value)
{
    maxMessageSize = value;
}

QList<QVariant *> AMQPAttach::getOfferedCapabilities() const
{
    return offeredCapabilities;
}

void AMQPAttach::setOfferedCapabilities(const QList<QVariant *> &value)
{
    offeredCapabilities = value;
}

QList<QVariant *> AMQPAttach::getDesiredCapabilities() const
{
    return desiredCapabilities;
}

void AMQPAttach::setDesiredCapabilities(const QList<QVariant *> &value)
{
    desiredCapabilities = value;
}

QMap<QVariant *, QVariant *> AMQPAttach::getProperties() const
{
    return properties;
}

void AMQPAttach::setProperties(const QMap<QVariant *, QVariant *> &value)
{
    properties = value;
}
