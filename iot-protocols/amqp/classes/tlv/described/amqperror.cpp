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

#include "amqperror.h"
#include "iot-protocols/amqp/classes/headerapi/amqpunwrapper.h"
#include "iot-protocols/amqp/classes/headerapi/amqpwrapper.h"
#include "iot-protocols/amqp/classes/constructors/amqpdescribedconstructor.h"
#include <QDebug>

AMQPError::AMQPError()
{
    this->condition = new AMQPErrorCode();
    this->descriptionString = QString();
    this->info = QMap<QVariant *, QVariant *>();
}

void AMQPError::fill(AMQPTLVList *list)
{
    if (list->getList().count() > 0) {
        TLVAMQP *element = list->getList().at(0);
        if (!element->isNull()) {
            QString name = AMQPUnwrapper::unwrapSymbol(element)->getValue();
            this->condition = new AMQPErrorCode(this->condition->EnumObject::getValue(name));
        }
    }

    if (list->getList().count() > 1) {
        TLVAMQP *element = list->getList().at(1);
        if (!element->isNull()) {
            this->descriptionString = AMQPUnwrapper::unwrapString(element);
        }
    }

    if (list->getList().count() > 2) {
        TLVAMQP *element = list->getList().at(2);
        if (!element->isNull()) {
            this->info = AMQPUnwrapper::unwrapMap(element);
        }
    }
}

void AMQPError::addInfo(QString key, QVariant *value)
{
    QVariant *keyVariant = AMQPSimpleType::symbolToVariant(new AMQPSymbol(key));
    this->info.insert(keyVariant, value);
}

AMQPTLVList *AMQPError::getList()
{
    AMQPTLVList *list = new AMQPTLVList();

    if (this->condition != NULL) {
        QVariant *symbol = AMQPSimpleType::symbolToVariant(new AMQPSymbol(this->condition->getName()));
        list->addElementWithIndex(0, AMQPWrapper::wrapObject(symbol));
    }
    if (!this->descriptionString.isNull()) {
        list->addElementWithIndex(1, AMQPWrapper::wrapString(this->descriptionString));
    }
    if (this->info.count() != 0) {
        list->addElementWithIndex(2, AMQPWrapper::wrapMap(this->info));
    }

    AMQPType *type = new AMQPType(AMQP_SMALL_ULONG_TYPE);
    ByteArray data = ByteArray();
    data.writeChar(0x1D);
    AMQPTLVFixed *fixed = new AMQPTLVFixed(type, data);
    AMQPType *constructorType = new AMQPType(list->getTypeValue());
    AMQPDescribedConstructor *constructor = new AMQPDescribedConstructor(constructorType, fixed);
    list->setConstructor(constructor);

    return list;
}

AMQPErrorCode *AMQPError::getCondition() const
{
    return condition;
}

void AMQPError::setCondition(AMQPErrorCode *value)
{
    condition = value;
}

QString AMQPError::getDescriptionString() const
{
    return descriptionString;
}

void AMQPError::setDescriptionString(const QString &value)
{
    descriptionString = value;
}

QMap<QVariant *, QVariant *> AMQPError::getInfo() const
{
    return info;
}
