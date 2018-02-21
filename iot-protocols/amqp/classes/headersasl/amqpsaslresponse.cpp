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

#include "amqpsaslresponse.h"
#include "iot-protocols/amqp/classes/constructors/amqpdescribedconstructor.h"
#include "iot-protocols/amqp/classes/headerapi/amqpwrapper.h"
#include "iot-protocols/amqp/classes/headerapi/amqpunwrapper.h"
#include <QDebug>

AMQPSASLResponse::AMQPSASLResponse() : AMQPHeader(new AMQPHeaderCode(AMQP_RESPONSE_HEADER_CODE))
{
    this->typeValue = 1;
}

int AMQPSASLResponse::getLength()
{
    int length = 8;
    AMQPTLVList *arguments = this->arguments();
    length += arguments->getLength();
    return length;
}

int AMQPSASLResponse::getType()
{
    return AMQP_RESPONSE_HEADER_CODE;
}

AMQPTLVList *AMQPSASLResponse::arguments()
{
    AMQPTLVList *list = new AMQPTLVList();

    if (this->response.getSize() == 0) {
        qDebug() << "AMQPSASLResponse::arguments::response";
        return new AMQPTLVList();
    }

    list->addElementWithIndex(0, AMQPWrapper::wrapBinary(this->response.getByteArray()));

    ByteArray data = ByteArray();
    data.writeChar(0x43);
    AMQPType *type = new AMQPType(AMQP_SMALL_ULONG_TYPE);
    AMQPTLVFixed *fixed = new AMQPTLVFixed(type, data);

    AMQPType *constructorType = new AMQPType(list->getTypeValue());
    AMQPDescribedConstructor *constructor = new AMQPDescribedConstructor(constructorType, fixed);

    list->setConstructor(constructor);
    return list;
}

void AMQPSASLResponse::fillArguments(AMQPTLVList *list)
{
    int size = list->getList().count();

    if (size == 0) {
        qDebug() << "AMQPSASLResponse::fillArguments::size == 0";
        return;
    }
    if (size > 1) {
        qDebug() << "AMQPSASLResponse::fillArguments::wrong_size";
        return;
    }
    if (size > 0) {
        TLVAMQP *element = list->getList().at(0);
        if (element->isNull()) {
            qDebug() << "AMQPSASLResponse::fillArguments::response";
            return;
        }
        this->response = AMQPUnwrapper::unwrapBinary(element);
    }
}

ByteArray AMQPSASLResponse::calcCramMD5(ByteArray challenge, QString name)
{
    if (challenge.getSize() != 0 && !name.isEmpty()) {

    } else {
        return ByteArray();
    }
    return ByteArray();
}

void AMQPSASLResponse::setCramMD5Response(ByteArray challenge, QString name)
{
    if (name.isNull()) {
        qDebug() << "AMQPSASLResponse::setCramMD5Response::name";
    }
    if (challenge.getSize() == 0) {
        qDebug() << "AMQPSASLResponse::setCramMD5Response::challenge";
    }
    this->response = this->calcCramMD5(challenge, name);
}



ByteArray AMQPSASLResponse::getResponse() const
{
    return response;
}

void AMQPSASLResponse::setResponse(const ByteArray &value)
{
    response = value;
}
