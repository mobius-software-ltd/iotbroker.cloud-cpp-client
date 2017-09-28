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

#include "amqpmessageformat.h"
#include "classes/bytearray.h"

AMQPMessageFormat::AMQPMessageFormat(long value)
{
    ByteArray data = ByteArray();
    data.writeInt(value);

    char *bytes = (char *)data.getByteArray().data();

    this->messageFormat = data.readUInt24();
    this->version = bytes[3] & 0xff;
}

AMQPMessageFormat::AMQPMessageFormat(int format, int version)
{
    this->messageFormat = format;
    this->version = version;
}

long AMQPMessageFormat::encode()
{
    ByteArray data = ByteArray();
    data.writeUInt24(this->messageFormat);
    data.writeChar(this->version);
    return data.readInt();
}
