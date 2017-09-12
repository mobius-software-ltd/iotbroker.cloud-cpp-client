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

#include "bytearray.h"

ByteArray::ByteArray()
{

}

ByteArray::ByteArray(QByteArray byteArray)
{
    this->byteArray = byteArray;
}

ByteArray::ByteArray(quint8 header, QByteArray byteArray)
{
    this->header = header;
    this->byteArray = byteArray;
}

QByteArray ByteArray::getByteArray()
{
    return this->byteArray;
}

char ByteArray::getLSB(short number)
{
    return (char)(number & 0x00FF);
}

char ByteArray::getMSB(short number)
{
    return (char)((number & 0xFF00) >> 8);
}

short ByteArray::readShort()
{
    char msb = static_cast<char>(this->byteArray.at(0));
    char lsb = static_cast<char>(this->byteArray.at(1));
    this->byteArray.remove(0, 2);
    return (msb << 8) | lsb;
}

char ByteArray::readChar()
{
    char ch = this->byteArray.at(0);
    this->byteArray.remove(0, 1);
    return ch;
}

QString ByteArray::readString()
{
    short length = readShort();
    QString string = QString::fromUtf8(this->byteArray.left(length));
    this->byteArray.remove(0, length);
    return string;
}

QString ByteArray::readStringWithLength(int length)
{
    QString string = QString::fromUtf8(this->byteArray.left(length));
    this->byteArray.remove(0, length);
    return string;
}

void ByteArray::writeShort(const short number)
{
    this->byteArray.append(getMSB(number));
    this->byteArray.append(getLSB(number));
}

void ByteArray::writeChar(const char ch)
{
    this->byteArray.append(ch);
}

void ByteArray::writeString(const QString &string)
{
    QByteArray array = string.toUtf8();
    if (array.size() > 65535) {
        qCritical(" -! qmqtt: String size bigger than %u bytes, truncate it!", 65535);
        array.resize(65535);
    }
    writeShort(array.size());
    this->byteArray.append(array);
}

void ByteArray::writeRawData(const QByteArray &data)
{
    this->byteArray.append(data);
}

int ByteArray::getSize()
{
    return this->byteArray.size();
}

void ByteArray::remove(int index, int length)
{
    this->byteArray.remove(index, length);
}

void ByteArray::write(QDataStream &stream)
{
    QByteArray lenbuf;

    if (!encodeLength(lenbuf, this->byteArray.size()))
    {
        qCritical("qmqtt: Control packet bigger than 256 MB, dropped!");
        return;
    }

    if (stream.writeRawData(lenbuf.data(), lenbuf.size()) != lenbuf.size())
    {
        qCritical("qmqtt: Control packet write error!");
        return;
    }
    if (stream.writeRawData(this->byteArray.data(), this->byteArray.size()) != this->byteArray.size())
    {
        qCritical("qmqtt: Control packet write error!");
    }
}

bool ByteArray::encodeLength(QByteArray &lenbuf, int length)
{
    lenbuf.clear();
    quint8 d;
    do {
        d = length % 128;
        length /= 128;
        if (length > 0) {
            d |= 0x80;
        }
        lenbuf.append(d);
    } while (length > 0);

    return lenbuf.size() <= 4;
}
