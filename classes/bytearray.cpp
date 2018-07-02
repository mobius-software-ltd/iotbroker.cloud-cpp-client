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

#include "bytearray.h"
#include <QDebug>

ByteArray::ByteArray()
{
    this->initSize = 0;
}

ByteArray::ByteArray(QByteArray byteArray)
{
    this->byteArray = byteArray;
    this->initSize = byteArray.size();
}

ByteArray::ByteArray(quint8 header, QByteArray byteArray)
{
    this->header = header;
    this->byteArray = byteArray;
    this->initSize = byteArray.size();
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

int ByteArray::readInt()
{
    return this->numberWithLength(4);
}

short ByteArray::readShort()
{
    char msb = static_cast<char>(this->byteArray.at(0));
    char lsb = static_cast<char>(this->byteArray.at(1));
    this->byteArray.remove(0, 2);    
    return (((msb & 0xFF) << 8) | (lsb & 0xFF));    //(msb << 8) | lsb;
}

char ByteArray::readChar()
{
    char ch = this->byteArray.at(0);
    this->byteArray.remove(0, 1);
    return ch;
}

int ByteArray::readUInt24()
{
    int length = 3;

    int value = 0;

    QByteArray array = this->byteArray.mid(0, length);
    this->byteArray.remove(0, length);

    char *bytes = (char *)array.data();

    for (int i = 0; i < length; i++) {
        value <<= 8;
        value |= bytes[i] & 0x00FF;
    }
    return (int)value;
}

long ByteArray::readLong()
{
    int length = sizeof(long);
    QByteArray array = this->byteArray.mid(0, length);
    this->byteArray.remove(0, length);
    return ((long *)array.data())[0];
}

float ByteArray::readFloat()
{
    int length = sizeof(float);
    QByteArray array = this->byteArray.mid(0, length);
    this->byteArray.remove(0, length);
    return ((float *)array.data())[0];
}

double ByteArray::readDouble()
{
    int length = sizeof(double);
    QByteArray array = this->byteArray.mid(0, length);
    this->byteArray.remove(0, length);
    return ((double *)array.data())[0];
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

QByteArray ByteArray::readRawData(int length)
{
    QByteArray array = this->byteArray.left(length);
    this->byteArray.remove(0, length);
    return array;
}

void ByteArray::writeInt(const int number)
{
    char a = (number >> 24) & 0XFF;
    char b = (number >> 16) & 0XFF;
    char c = (number >> 8) & 0XFF;
    char d = number & 0XFF;
    this->byteArray.append(a);
    this->byteArray.append(b);
    this->byteArray.append(c);
    this->byteArray.append(d);
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

void ByteArray::writeUInt24(int number)
{
    int length = 3;

    char *bytes = (char *)malloc(sizeof(char) * length);

    for (int i = 0, bitShift = 16; i < length; i++, bitShift -= 8) {
        if (bitShift == 0) {
            bytes[i] = (char)(number & 0x00FF);
        } else {
            bytes[i] = (char)((number >> bitShift) & 0x00FF);
        }
    }
    this->byteArray.append(bytes, length);
    free(bytes);
}

void ByteArray::writeLong(long number)
{
    QByteArray data = QByteArray(reinterpret_cast<const char*>(&number), sizeof(long));
    this->byteArray.append(data);
}

void ByteArray::writeFloat(float number)
{
    QByteArray data = QByteArray(reinterpret_cast<const char*>(&number), sizeof(float));
    this->byteArray.append(data);
}

void ByteArray::writeDouble(double number)
{
    QByteArray data = QByteArray(reinterpret_cast<const char*>(&number), sizeof(double));
    this->byteArray.append(data);
}

int ByteArray::numberWithLength(int length)
{
    if (this->getSize() < length)
        return 0;

    int number = 0;
    char *charArray = this->byteArray.data();

    for (int i = 0; i < length; i++) {
        number <<= 8;
        number |= charArray[i] & 0x00FF;
    }
    this->byteArray.remove(0, length);
    return number;
}

int ByteArray::getSize()
{
    return this->byteArray.size();
}

void ByteArray::remove(int index, int length)
{
    this->byteArray.remove(index, length);
}

ByteArray ByteArray::subFrom(int index, int length)
{
    QByteArray copy = QByteArray(this->byteArray);
    QByteArray sub = copy.mid(index, length);
    return ByteArray(sub);
}

int ByteArray::readerIndex()
{
    return this->initSize - this->getSize();
}

int ByteArray::getInitSize() const
{
    return initSize;
}

void ByteArray::fixSizeAsInitial()
{
    this->initSize = this->getSize();
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
