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

#include "amqpmodified.h"
#include "iot-protocols/amqp/classes/headerapi/amqpunwrapper.h"
#include "iot-protocols/amqp/classes/headerapi/amqpwrapper.h"
#include "iot-protocols/amqp/classes/constructors/amqpdescribedconstructor.h"
#include <QDebug>

AMQPModified::AMQPModified()
{
    this->messageAnnotations = QMap<QVariant *, QVariant *>();
}

void AMQPModified::fill(AMQPTLVList *list)
{
    if (list->getList().count() > 0) {
        TLVAMQP *element = list->getList().at(0);
        if (!element->isNull()) {
            this->deliveryFailed = new QVariant(AMQPUnwrapper::unwrapBool(element));
        }
    }
    if (list->getList().count() > 1) {
        TLVAMQP *element = list->getList().at(1);
        if (!element->isNull()) {
            this->undeliverableHere = new QVariant(AMQPUnwrapper::unwrapBool(element));
        }
    }
    if (list->getList().count() > 2) {
        TLVAMQP *element = list->getList().at(2);
        if (!element->isNull()) {
            this->messageAnnotations = AMQPUnwrapper::unwrapMap(element);
        }
    }
}

AMQPTLVList *AMQPModified::getList()
{
    AMQPTLVList *list = new AMQPTLVList();

    if (this->deliveryFailed != NULL) {
        list->addElementWithIndex(0, AMQPWrapper::wrapBool(this->deliveryFailed->toBool()));
    }
    if (this->undeliverableHere != NULL) {
        list->addElementWithIndex(1, AMQPWrapper::wrapBool(this->undeliverableHere->toBool()));
    }
    if (this->messageAnnotations.count() != 0) {
        list->addElementWithIndex(2, AMQPWrapper::wrapMap(this->messageAnnotations));
    }

    ByteArray data = ByteArray();
    data.writeChar(0x27);
    AMQPType *type = new AMQPType(AMQP_SMALL_ULONG_TYPE);
    AMQPTLVFixed *fixed = new AMQPTLVFixed(type, data);

    AMQPType *constructorType = new AMQPType(list->getTypeValue());
    AMQPDescribedConstructor *constructor = new AMQPDescribedConstructor(constructorType, fixed);

    list->setConstructor(constructor);

    return list;
}

void AMQPModified::addMessageAnnotation(QString key, QVariant *value)
{
    if (key.length() < 2) {
        return;
    }

    QString suffix = key.mid(0, 2);
    if (suffix != QString("x-")) {
        qDebug() << "AMQPModified::addMessageAnnotation";
        return;
    }

    QVariant *keyVariant = AMQPSimpleType::symbolToVariant(new AMQPSymbol(key));
    this->messageAnnotations.insert(keyVariant, value);
}



QVariant *AMQPModified::getDeliveryFailed() const
{
    return deliveryFailed;
}

void AMQPModified::setDeliveryFailed(QVariant *value)
{
    deliveryFailed = value;
}

QVariant *AMQPModified::getUndeliverableHere() const
{
    return undeliverableHere;
}

void AMQPModified::setUndeliverableHere(QVariant *value)
{
    undeliverableHere = value;
}

QMap<QVariant *, QVariant *> AMQPModified::getMessageAnnotations() const
{
    return messageAnnotations;
}

void AMQPModified::setMessageAnnotations(const QMap<QVariant *, QVariant *> &value)
{
    messageAnnotations = value;
}
