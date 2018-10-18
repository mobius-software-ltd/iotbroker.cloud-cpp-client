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

#include "amqptarget.h"
#include "iot-protocols/amqp/classes/wrappers/amqpsymbol.h"
#include "iot-protocols/amqp/classes/constructors/amqpdescribedconstructor.h"
#include "iot-protocols/amqp/classes/tlv/api/amqptlvfactory.h"
#include "iot-protocols/amqp/classes/tlv/fixed/amqptlvfixed.h"
#include "iot-protocols/amqp/classes/headerapi/amqpwrapper.h"
#include "iot-protocols/amqp/classes/headerapi/amqpunwrapper.h"
#include "iot-protocols/amqp/classes/headerapi/amqpfactory.h"
#include <QDebug>
#include <QVariant>

AMQPTarget::AMQPTarget()
{
    this->expiryPeriod = NULL;
    this->dynamicNodeProperties = QMap<QVariant *, QVariant *>();
    this->capabilities = QList<QVariant *>();
}

AMQPTLVList *AMQPTarget::getlist()
{
    AMQPTLVList *list = new AMQPTLVList();
    
    qDebug() << "######1";
    if (!this->address.isEmpty()) {
        list->addElementWithIndex(0, AMQPWrapper::wrapString(this->address));
    }
    qDebug() << "######2";
    if (this->durable != NULL) {
        list->addElementWithIndex(1, AMQPWrapper::wrapUInt(this->durable->getValue()));
    }
    qDebug() << "######3";
    if (this->expiryPeriod != NULL) {
        AMQPSymbol *symbol = new AMQPSymbol(this->expiryPeriod->getName());
        list->addElementWithIndex(2, AMQPWrapper::wrapSymbol(symbol));
    }
    qDebug() << "######4";
    if (this->timeout != NULL) {
        list->addElementWithIndex(3, AMQPWrapper::wrapUInt(this->timeout->toUInt()));
    }
    qDebug() << "######5";
    if (this->dynamic != NULL) {
        list->addElementWithIndex(4, AMQPWrapper::wrapBool(this->dynamic->toBool()));
    }
    qDebug() << "######6";
    if (this->dynamicNodeProperties.count() != 0) {
        if (this->dynamic != NULL) {
            if (this->dynamic->toBool()) {
                list->addElementWithIndex(5, AMQPWrapper::wrapMap(this->dynamicNodeProperties));
            } else {
                qDebug() << "AMQPTarget::getList::dynamic";
            }
        } else {
            qDebug() << "AMQPTarget::getList::dynamicNodeProperties";
        }
    }

    if (this->capabilities.count() != 0) {
        list->addElementWithIndex(6, AMQPWrapper::wrapList(this->capabilities));
    }
    ByteArray data = ByteArray();
    data.writeChar(0x29);
    AMQPType *type = new AMQPType(AMQP_SMALL_ULONG_TYPE);
    AMQPTLVFixed *fixed = new AMQPTLVFixed(type, data);

    AMQPType *constructorType = new AMQPType(list->getTypeValue());
    AMQPDescribedConstructor *constructor = new AMQPDescribedConstructor(constructorType, fixed);

    list->setConstructor(constructor);

    return list;
}

void AMQPTarget::fill(AMQPTLVList *list)
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
            AMQPTerminusExpiryPolicy *expiryPolicy = new AMQPTerminusExpiryPolicy();
            expiryPolicy = new AMQPTerminusExpiryPolicy(expiryPolicy->EnumObject::getValue(symbol->getValue()));
            this->expiryPeriod = expiryPolicy;
        }
    }
    if (list->getList().count() > 3) {
        TLVAMQP *element = list->getList().at(3);
        if (!element->isNull()) {
            this->timeout = new QVariant((unsigned int)AMQPUnwrapper::unwrapUInt(element));
        }
    }
    if (list->getList().count() > 4) {
        TLVAMQP *element = list->getList().at(4);
        if (!element->isNull()) {
            this->dynamic = new QVariant(AMQPUnwrapper::unwrapBool(element));
        }
    }
    if (list->getList().count() > 5) {
        TLVAMQP *element = list->getList().at(5);
        if (!element->isNull()) {
            if (this->dynamic != NULL) {
                if (this->dynamic->toBool()) {
                    this->dynamicNodeProperties = AMQPUnwrapper::unwrapMap(element);
                } else {
                    qDebug() << "AMQPTarget::fill::dynamic";
                }
            } else {
                qDebug() << "AMQPTarget::fill::dynamicNodeProperties";
            }
        }
    }
    if (list->getList().count() > 6) {
        TLVAMQP *element = list->getList().at(6);
        if (!element->isNull()) {
            this->capabilities = AMQPUnwrapper::unwrapList(element);
        }
    }
}

void AMQPTarget::addDynamicNodeProperties(QString key, QVariant *value)
{
    QVariant *keyVariant = AMQPSimpleType::symbolToVariant(new AMQPSymbol(key));
    this->dynamicNodeProperties.insert(keyVariant, value);
}

void AMQPTarget::addCapabilities(QList<QString> array)
{
    foreach (QString item, array) {
        QVariant *itemVariant = AMQPSimpleType::symbolToVariant(new AMQPSymbol(item));
        this->capabilities.append(itemVariant);
    }
}


QString AMQPTarget::getAddress() const
{
    return address;
}

void AMQPTarget::setAddress(const QString &value)
{
    address = value;
}

AMQPTerminusDurability *AMQPTarget::getDurable() const
{
    return durable;
}

void AMQPTarget::setDurable(AMQPTerminusDurability *value)
{
    durable = value;
}

AMQPTerminusExpiryPolicy *AMQPTarget::getExpiryPeriod() const
{
    return expiryPeriod;
}

void AMQPTarget::setExpiryPeriod(AMQPTerminusExpiryPolicy *value)
{
    expiryPeriod = value;
}

QVariant *AMQPTarget::getTimeout() const
{
    return timeout;
}

void AMQPTarget::setTimeout(QVariant *value)
{
    timeout = value;
}

QVariant *AMQPTarget::getDynamic() const
{
    return dynamic;
}

void AMQPTarget::setDynamic(QVariant *value)
{
    dynamic = value;
}

QMap<QVariant *, QVariant *> AMQPTarget::getDynamicNodeProperties() const
{
    return dynamicNodeProperties;
}

void AMQPTarget::setDynamicNodeProperties(const QMap<QVariant *, QVariant *> &value)
{
    dynamicNodeProperties = value;
}

QList<QVariant *> AMQPTarget::getCapabilities() const
{
    return capabilities;
}

void AMQPTarget::setCapabilities(const QList<QVariant *> &value)
{
    capabilities = value;
}
