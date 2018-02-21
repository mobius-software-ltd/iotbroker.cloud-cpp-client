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

#include "amqpdecimal.h"

AMQPDecimal::AMQPDecimal()
{
    this->value = ByteArray();
}

AMQPDecimal::AMQPDecimal(ByteArray value) : AMQPDecimal()
{
    this->value = value;
}

AMQPDecimal::AMQPDecimal(char value) : AMQPDecimal()
{
    this->value.writeChar(value);
}

AMQPDecimal::AMQPDecimal(short value) : AMQPDecimal()
{
    this->value.writeShort(value);
}

AMQPDecimal::AMQPDecimal(int value) : AMQPDecimal()
{
    this->value.writeInt(value);
}

AMQPDecimal::AMQPDecimal(long value) : AMQPDecimal()
{
    this->value.writeLong(value);
}

AMQPDecimal::AMQPDecimal(float value) : AMQPDecimal()
{
    this->value.writeFloat(value);
}

AMQPDecimal::AMQPDecimal(double value) : AMQPDecimal()
{
    this->value.writeDouble(value);
}

char AMQPDecimal::getByte()
{
    return this->value.readChar();
}

short AMQPDecimal::getShortNumber()
{
    return this->value.readShort();
}

int AMQPDecimal::getIntNumber()
{
    return this->value.readInt();
}

long AMQPDecimal::getLongNumber()
{
    return this->value.readLong();
}

float AMQPDecimal::getFloatNumber()
{
    return this->value.readFloat();
}

double AMQPDecimal::getDoubleNumber()
{
    return this->value.readDouble();
}



ByteArray AMQPDecimal::getValue() const
{
    return value;
}
