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

#include "amqptlvfixed.h"

AMQPTLVFixed::AMQPTLVFixed(AMQPType *type, ByteArray value) : TLVAMQP(new AMQPSimpleConstructor(type))
{
    this->value = value;
}

ByteArray AMQPTLVFixed::getData()
{
    ByteArray bytes = ByteArray();
    bytes.writeRawData(this->constructor->getData().getByteArray());

    if (this->value.getSize() > 0) {
        bytes.writeRawData(this->value.getByteArray());
    }

    return bytes;
}

int AMQPTLVFixed::getLength()
{
    return this->value.getSize() + this->constructor->getLength();
}

ByteArray AMQPTLVFixed::getValue()
{
    return this->value;
}

AMQPTypes AMQPTLVFixed::getType()
{
    return (AMQPTypes)this->constructor->getType()->getValue();
}

void AMQPTLVFixed::setType(AMQPTypes type)
{
    this->constructor->setType(new AMQPType(type));
}

QString AMQPTLVFixed::description()
{
    return this->constructor->getType()->getName();
}
