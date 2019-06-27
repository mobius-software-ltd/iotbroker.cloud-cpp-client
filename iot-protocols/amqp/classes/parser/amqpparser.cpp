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

#include "amqpparser.h"
#include <QObject>
#include <QDebug>

AMQPParser::AMQPParser()
{

}

int AMQPParser::next(ByteArray buffer)
{
    ByteArray data = ByteArray(buffer.getByteArray());

    int length = data.readInt();

    if (length == 1095586128) {
        int protocolId = data.readChar();
        int versionMajor = data.readChar();
        int versionMinor = data.readChar();
        int versionRevision = data.readChar();
        if ((protocolId == AMQPProtocolId || protocolId == AMQPProtocolIdTLS || protocolId == AMQPProtocolIdSASL) &&
                versionMajor == 1 && versionMinor == 0 && versionRevision == 0) {
            return 8;
        } else {
            return -1;
        }
    }
    return length;
}

AMQPHeader *AMQPParser::decode(ByteArray buffer)
{
    if(buffer.getSize()<8)
        return NULL;
    ByteArray data = ByteArray(buffer.getByteArray());
    if (data.readStringWithLength(4) == AMQPProtocolName) {

        AMQPProtoHeader *protoHeader = new AMQPProtoHeader();
        protoHeader->setProtocolId((AMQPProtocolIds)data.readChar());
        protoHeader->setVersionMajor(data.readChar());
        protoHeader->setVersionMinor(data.readChar());
        protoHeader->setVersionRevision(data.readChar());
        return protoHeader;
    }

    buffer.fixSizeAsInitial();

    int length = buffer.readInt() & 0xffffffffL;
    int doff = buffer.readChar() & 0xff;
    int type = buffer.readChar() & 0xff;
    int chennel = buffer.readShort() & 0xffff;

    if (length == 8 && doff == 2 && (type == 0 || type == 1) && chennel == 0) {
        if (buffer.getInitSize() - buffer.readerIndex() == 0) {
            return new AMQPPing();
        } else {
            qDebug() << "Received malformed ping-header with invalid length";
            return NULL;
        }
    }

    if (length == 1095586128 && (doff == 3 || doff == 0) && type == 1 && chennel == 0) {
        if (buffer.getInitSize() - buffer.readerIndex() == 0) {
            return new AMQPPing();
        } else {
            qDebug() << "Received malformed protocol-header with invalid length";
            return NULL;
        }
    }

    if (length != (buffer.getInitSize() - buffer.readerIndex()) + 8) {
        qDebug() << "Received malformed header with invalid length";
        return NULL;
    }

    AMQPHeader *header = NULL;

    if (type == 0) {
        header = AMQPFactory::amqp(&buffer);
    } else if (type == 1) {
        header = AMQPFactory::sasl(&buffer);
    } else {
        qDebug() << "Received malformed header with invalid type";
        return NULL;
    }

    if (header->getCode()->getValue() == AMQP_TRANSFER_HEADER_CODE) {
        while ((buffer.getInitSize() - buffer.readerIndex()) > 0) {
            QList<AMQPSection *> sections = QList<AMQPSection *>();
            sections.append(AMQPFactory::section(&buffer));
            ((AMQPTransfer *)header)->addSections(sections);
        }
    }

    return header;
}

ByteArray AMQPParser::encode(AMQPHeader *header)
{
    ByteArray data = ByteArray();

    if (qobject_cast<AMQPProtoHeader *>(header) != NULL) {
        data.writeRawData(AMQPProtocolName.toUtf8());
        data.writeChar(((AMQPProtoHeader *)header)->getProtocolId());
        data.writeChar(((AMQPProtoHeader *)header)->getVersionMajor());
        data.writeChar(((AMQPProtoHeader *)header)->getVersionMinor());
        data.writeChar(((AMQPProtoHeader *)header)->getVersionRevision());
        return data;
    }

    if (qobject_cast<AMQPPing *>(header) != NULL) {
        data.writeInt(8);
        data.writeChar(header->getDoff());
        data.writeChar(header->getType());
        data.writeShort(header->getChannel());
        return data;
    }

    data.writeInt(header->getLength());
    data.writeChar(header->getDoff());
    data.writeChar(header->getTypeValue());
    data.writeShort(header->getChannel());

    data.writeRawData(header->arguments()->getData().getByteArray());

    if (header->getCode()->getValue() == AMQP_TRANSFER_HEADER_CODE) {
        QList<AMQPSection *> sections = ((AMQPTransfer *)header)->getSections().values();

        foreach (AMQPSection *item, sections) {
            data.writeRawData(item->getValue()->getData().getByteArray());
        }
    }

    return data;
}
