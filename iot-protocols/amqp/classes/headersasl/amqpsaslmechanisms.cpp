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

#include "amqpsaslmechanisms.h"
#include "iot-protocols/amqp/classes/constructors/amqpdescribedconstructor.h"
#include "iot-protocols/amqp/classes/headerapi/amqpwrapper.h"
#include "iot-protocols/amqp/classes/headerapi/amqpunwrapper.h"
#include <QDebug>

AMQPSASLMechanisms::AMQPSASLMechanisms() : AMQPHeader(new AMQPHeaderCode(AMQP_MECHANISMS_HEADER_CODE))
{
    this->mechanisms = QList<QVariant *>();
    this->typeValue = 1;
}

int AMQPSASLMechanisms::getLength()
{
    int length = 8;
    AMQPTLVList *arguments = this->arguments();
    length += arguments->getLength();
    return length;
}

int AMQPSASLMechanisms::getType()
{
    return AMQP_MECHANISMS_HEADER_CODE;
}

AMQPTLVList *AMQPSASLMechanisms::arguments()
{
    AMQPTLVList *list = new AMQPTLVList();

    if (this->mechanisms.count() == 0) {
        qDebug() << "AMQPSASLMechanisms::arguments::mechanisms";
        return new AMQPTLVList();
    }

    list->addElementWithIndex(0, AMQPWrapper::wrapArray(this->mechanisms));

    ByteArray data = ByteArray();
    data.writeChar(0x40);
    AMQPType *type = new AMQPType(AMQP_SMALL_ULONG_TYPE);
    AMQPTLVFixed *fixed = new AMQPTLVFixed(type, data);

    AMQPType *constructorType = new AMQPType(list->getTypeValue());
    AMQPDescribedConstructor *constructor = new AMQPDescribedConstructor(constructorType, fixed);

    list->setConstructor(constructor);
    return list;
}

void AMQPSASLMechanisms::fillArguments(AMQPTLVList *list)
{
    if (list->getList().count() > 0) {
        TLVAMQP *element = list->getList().at(0);
        if (!element->isNull()) {
            this->mechanisms = AMQPUnwrapper::unwrapArray(element);
        }
    }
}

void AMQPSASLMechanisms::addMechanism(QString value)
{
    QVariant *variant = AMQPSimpleType::symbolToVariant(new AMQPSymbol(value));
    this->mechanisms.append(variant);
}



QList<QVariant *> AMQPSASLMechanisms::getMechanisms() const
{
    return mechanisms;
}

void AMQPSASLMechanisms::setMechanisms(const QList<QVariant *> &value)
{
    mechanisms = value;
}
