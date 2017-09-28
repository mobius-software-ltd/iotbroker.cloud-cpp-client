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

#include "amqpdescribedconstructor.h"

AMQPDescribedConstructor::AMQPDescribedConstructor(AMQPType *type, TLVAMQP *descriptor) : AMQPSimpleConstructor(type)
{
    this->descriptor = descriptor;
}

ByteArray AMQPDescribedConstructor::getData() const
{
    ByteArray data = ByteArray();
    data.writeChar(0);
    data.writeRawData(this->descriptor->getData().getByteArray());
    data.writeChar(this->type->getValue());
    return data;
}

int AMQPDescribedConstructor::getLength() const
{
    return this->descriptor->getData().getSize() + 2;
}

int AMQPDescribedConstructor::getDescriptorCode() const
{
    QByteArray data = this->descriptor->getData().getByteArray();
    return data.at(1);
}
