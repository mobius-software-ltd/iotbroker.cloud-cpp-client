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

#include "amqpsource.h"
#include "iot-protocols/amqp/classes/wrappers/amqpsymbol.h"
#include "iot-protocols/amqp/classes/constructors/amqpdescribedconstructor.h"
#include "iot-protocols/amqp/classes/tlv/api/amqptlvfactory.h"
#include "iot-protocols/amqp/classes/tlv/fixed/amqptlvfixed.h"
#include "iot-protocols/amqp/classes/headerapi/amqpwrapper.h"
#include "iot-protocols/amqp/classes/headerapi/amqpunwrapper.h"
#include "iot-protocols/amqp/classes/headerapi/amqpfactory.h"

#include <QDebug>

AMQPSource::AMQPSource()
{
    this->dynamicNodeProperties = QMap<QVariant *, QVariant *>();
    this->filter = QMap<QVariant *, QVariant *>();
    this->outcomes = QList<QVariant *>();
    this->capabilities = QList<QVariant *>();
}

AMQPTLVList *AMQPSource::getList()
{
    AMQPTLVList *list = new AMQPTLVList();

    if (!this->address.isEmpty()) {
        list->addElementWithIndex(0, AMQPWrapper::wrapString(this->address));
    }
    if (this->durable != NULL) {
        list->addElementWithIndex(1, AMQPWrapper::wrapUInt(this->durable->getValue()));
    }
    if (this->expiryPeriod != NULL) {
        AMQPSymbol *symbol = new AMQPSymbol(this->expiryPeriod->getName());
        list->addElementWithIndex(2, AMQPWrapper::wrapSymbol(symbol));
    }
    if (this->timeout != NULL) {
        list->addElementWithIndex(3, AMQPWrapper::wrapUInt(this->timeout->toUInt()));
    }
    if (this->dynamic != NULL) {
        list->addElementWithIndex(4, AMQPWrapper::wrapBool(this->timeout->toBool()));
    }
    if (this->dynamicNodeProperties.count() != 0) {
        if (this->dynamic != NULL) {
            if (this->dynamic->toBool() == true) {
                list->addElementWithIndex(5, AMQPWrapper::wrapMap(this->dynamicNodeProperties));
            } else {
                qDebug() << "AMQPSource::getList::dynamicNodeProperties";
            }
        } else {
            qDebug() << "AMQPSource::getList::dynamic";
        }
    }
    if (this->distributionMode != NULL) {
        AMQPSymbol *symbol = new AMQPSymbol(this->distributionMode->getName());
        list->addElementWithIndex(6, AMQPWrapper::wrapSymbol(symbol));
    }
    if (this->filter.count() != 0) {
        list->addElementWithIndex(7, AMQPWrapper::wrapMap(this->filter));
    }
    if (this->defaultOutcome != NULL) {
        list->addElementWithIndex(8, this->defaultOutcome->getList());
    }
    if (this->outcomes.count() != 0) {
        list->addElementWithIndex(9, AMQPWrapper::wrapArray(this->outcomes));
    }
    if (this->capabilities.count() != 0) {
        list->addElementWithIndex(9, AMQPWrapper::wrapList(this->capabilities));
    }

    ByteArray data = ByteArray();
    data.writeChar(0x28);
    AMQPType *type = new AMQPType(AMQP_SMALL_ULONG_TYPE);
    AMQPTLVFixed *fixed = new AMQPTLVFixed(type, data);

    AMQPType *constructorType = new AMQPType(list->getTypeValue());
    AMQPDescribedConstructor *constructor = new AMQPDescribedConstructor(constructorType, fixed);

    list->setConstructor(constructor);

    return list;
}

void AMQPSource::fill(AMQPTLVList *list)
{
    if (list->getList().count() > 0) {
        TLVAMQP *element = list->getList().at(0);
        if (!element->isNull()) {
            this->address = AMQPUnwrapper::unwrapString(element);
        }
    }
    if (list->getList().count() > 1) {
        TLVAMQP *element = list->getList().at(1);
        if (!element->isNull()) {
            int number = AMQPUnwrapper::unwrapUInt(element);
            this->durable = new AMQPTerminusDurability(number);
        }
    }
    if (list->getList().count() > 2) {
        TLVAMQP *element = list->getList().at(2);
        if (!element->isNull()) {
            AMQPSymbol *symbol = AMQPUnwrapper::unwrapSymbol(element);
            AMQPTerminusExpiryPolicy *policy = new AMQPTerminusExpiryPolicy();
            policy = new AMQPTerminusExpiryPolicy(policy->EnumObject::getValue(symbol->getValue()));
            this->expiryPeriod = policy;
        }
    }
    if (list->getList().count() > 3) {
        TLVAMQP *element = list->getList().at(3);
        if (!element->isNull()) {
            int number = AMQPUnwrapper::unwrapUInt(element);
            this->timeout = new QVariant(number);
        }
    }
    if (list->getList().count() > 4) {
        TLVAMQP *element = list->getList().at(4);
        if (!element->isNull()) {
            int number = AMQPUnwrapper::unwrapBool(element);
            this->dynamic = new QVariant(number);
        }
    }
    if (list->getList().count() > 5) {
        TLVAMQP *element = list->getList().at(5);
        if (!element->isNull()) {
            if (this->dynamic != NULL) {
                if (this->dynamic->toBool()) {
                    this->dynamicNodeProperties = AMQPUnwrapper::unwrapMap(element);
                } else {
                    qDebug() << "AMQPSource::fill::dynamicNodeProperties";
                }
            } else {
                qDebug() << "AMQPSource::fill::dynamic";
            }
        }
    }
    if (list->getList().count() > 6) {
        TLVAMQP *element = list->getList().at(6);
        if (!element->isNull()) {
            AMQPSymbol *symbol = AMQPUnwrapper::unwrapSymbol(element);
            AMQPDistributionMode *mode = new AMQPDistributionMode();
            mode = new AMQPDistributionMode(mode->EnumObject::getValue(symbol->getValue()));
        }
    }
    if (list->getList().count() > 7) {
        TLVAMQP *element = list->getList().at(7);
        if (!element->isNull()) {
            this->filter = AMQPUnwrapper::unwrapMap(element);
        }
    }
    if (list->getList().count() > 8) {
        TLVAMQP *element = list->getList().at(8);
        if (!element->isNull()) {
            AMQPType *code = new AMQPType(element->getTypeValue());
            if (code->getValue() != AMQP_LIST0_TYPE && code->getValue() != AMQP_LIST8_TYPE && code->getValue() != AMQP_LIST32_TYPE) {
                qDebug() << "AMQPSource::fill::wrong_type";
            }
            this->defaultOutcome = AMQPFactory::outcome((AMQPTLVList *)element);
            this->defaultOutcome->fill((AMQPTLVList *)element);
        }
    }
    if (list->getList().count() > 9) {
        TLVAMQP *element = list->getList().at(9);
        if (!element->isNull()) {
            this->outcomes = AMQPUnwrapper::unwrapArray(element);
        }
    }
    if (list->getList().count() > 10) {
        TLVAMQP *element = list->getList().at(10);
        if (!element->isNull()) {
            this->capabilities = AMQPUnwrapper::unwrapList(element);
        }
    }
}

void AMQPSource::addDynamicNodeProperties(QString key, QVariant *value)
{
    QVariant *keyVariant = AMQPSimpleType::symbolToVariant(new AMQPSymbol(key));
    this->dynamicNodeProperties.insert(keyVariant, value);
}

void AMQPSource::addFilter(QString key, QVariant *value)
{
    QVariant *keyVariant = AMQPSimpleType::symbolToVariant(new AMQPSymbol(key));
    this->filter.insert(keyVariant, value);
}

void AMQPSource::addOutcomes(QList<QString> array)
{
    foreach (QString item, array) {
        QVariant *itemVariant = AMQPSimpleType::symbolToVariant(new AMQPSymbol(item));
        this->outcomes.append(itemVariant);
    }
}

void AMQPSource::addCapabilities(QList<QString> array)
{
    foreach (QString item, array) {
        QVariant *itemVariant = AMQPSimpleType::symbolToVariant(new AMQPSymbol(item));
        this->capabilities.append(itemVariant);
    }
}


QString AMQPSource::getAddress() const
{
    return address;
}

void AMQPSource::setAddress(const QString &value)
{
    address = value;
}

AMQPTerminusDurability *AMQPSource::getDurable() const
{
    return durable;
}

void AMQPSource::setDurable(AMQPTerminusDurability *value)
{
    durable = value;
}

AMQPTerminusExpiryPolicy *AMQPSource::getExpiryPeriod() const
{
    return expiryPeriod;
}

void AMQPSource::setExpiryPeriod(AMQPTerminusExpiryPolicy *value)
{
    expiryPeriod = value;
}

QVariant *AMQPSource::getTimeout() const
{
    return timeout;
}

void AMQPSource::setTimeout(QVariant *value)
{
    timeout = value;
}

QVariant *AMQPSource::getDynamic() const
{
    return dynamic;
}

void AMQPSource::setDynamic(QVariant *value)
{
    dynamic = value;
}

QMap<QVariant *, QVariant *> AMQPSource::getDynamicNodeProperties() const
{
    return dynamicNodeProperties;
}

void AMQPSource::setDynamicNodeProperties(const QMap<QVariant *, QVariant *> &value)
{
    dynamicNodeProperties = value;
}

AMQPDistributionMode *AMQPSource::getDistributionMode() const
{
    return distributionMode;
}

void AMQPSource::setDistributionMode(AMQPDistributionMode *value)
{
    distributionMode = value;
}

QMap<QVariant *, QVariant *> AMQPSource::getFilter() const
{
    return filter;
}

void AMQPSource::setFilter(const QMap<QVariant *, QVariant *> &value)
{
    filter = value;
}

AMQPOutcome *AMQPSource::getDefaultOutcome() const
{
    return defaultOutcome;
}

void AMQPSource::setDefaultOutcome(AMQPOutcome *value)
{
    defaultOutcome = value;
}

QList<QVariant *> AMQPSource::getOutcomes() const
{
    return outcomes;
}

void AMQPSource::setOutcomes(const QList<QVariant *> &value)
{
    outcomes = value;
}

QList<QVariant *> AMQPSource::getCapabilities() const
{
    return capabilities;
}

void AMQPSource::setCapabilities(const QList<QVariant *> &value)
{
    capabilities = value;
}
