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

#include "amqptlvvariable.h"

AMQPTLVVariable::AMQPTLVVariable(AMQPType *type, ByteArray value) : TLVAMQP(new AMQPSimpleConstructor(type))
{
    this->value = value;
    this->width = (this->value.getSize() > 255) ? 4 : 1;
}

ByteArray AMQPTLVVariable::getData()
{
    ByteArray widthData = ByteArray();

    if (this->width == 1) {
        widthData.writeChar(this->value.getSize());
    } else if (this->width == 4) {
        widthData.writeInt(this->value.getSize());
    }

    ByteArray bytes = ByteArray();
    bytes.writeRawData(this->constructor->getData().getByteArray());
    bytes.writeRawData(widthData.getByteArray());

    if (this->getValue().getSize() > 0) {
        bytes.writeRawData(this->getValue().getByteArray());
    }

    return bytes;
}

int AMQPTLVVariable::getLength()
{
    return this->value.getSize() + this->constructor->getLength() + this->width;
}

ByteArray AMQPTLVVariable::getValue()
{
    return this->value;
}

QString AMQPTLVVariable::description()
{
    return QString(this->getValue().getByteArray());
}
