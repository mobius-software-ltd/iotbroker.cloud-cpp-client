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

#include "amqpprotoheader.h"

AMQPProtoHeader::AMQPProtoHeader() : AMQPHeader()
{

}

AMQPProtoHeader::AMQPProtoHeader(AMQPProtocolIds protocolId) : AMQPProtoHeader()
{
    this->protocolId = protocolId;
}

int AMQPProtoHeader::getLength()
{
    return 8;
}

int AMQPProtoHeader::getType()
{
    return AMQP_PROTOCOL_HEADER_CODE;
}

ByteArray AMQPProtoHeader::getBytes()
{
    ByteArray data = ByteArray();
    data.writeRawData(AMQPProtocolName.toUtf8());
    data.writeChar(this->protocolId);
    data.writeChar(this->versionMajor);
    data.writeChar(this->versionMinor);
    data.writeChar(this->versionRevision);
    return data;
}

AMQPTLVList *AMQPProtoHeader::arguments()
{
    return NULL;
}

void AMQPProtoHeader::fillArguments(AMQPTLVList *list)
{
    Q_UNUSED(list);
    return;
}

AMQPProtocolIds AMQPProtoHeader::getProtocolId() const
{
    return protocolId;
}

void AMQPProtoHeader::setProtocolId(const AMQPProtocolIds &value)
{
    protocolId = value;
}

int AMQPProtoHeader::getVersionMajor() const
{
    return versionMajor;
}

void AMQPProtoHeader::setVersionMajor(int value)
{
    versionMajor = value;
}

int AMQPProtoHeader::getVersionMinor() const
{
    return versionMinor;
}

void AMQPProtoHeader::setVersionMinor(int value)
{
    versionMinor = value;
}

int AMQPProtoHeader::getVersionRevision() const
{
    return versionRevision;
}

void AMQPProtoHeader::setVersionRevision(int value)
{
    versionRevision = value;
}
