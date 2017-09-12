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

#ifndef BYTEARRAY_H
#define BYTEARRAY_H

#include <QByteArray>
#include <QString>
#include <QDataStream>

/**
 * @brief The ByteArray class
 */

class ByteArray
{

private:
    QByteArray byteArray;
    quint8 header;

public:
    ByteArray();
    ByteArray(quint8 header, QByteArray byteArray);
    ByteArray(QByteArray byteArray);

    QByteArray getByteArray();

    char getLSB(short number);
    char getMSB(short number);

    short readShort();
    char readChar();
    QString readString();
    QString readStringWithLength(int length);

    void writeShort(const short number);
    void writeChar(const char ch);
    void writeString(const QString &string);
    void writeRawData(const QByteArray &data);

    void write(QDataStream &stream);

    bool encodeLength(QByteArray &lenbuf, int length);

    int getSize();

    void remove(int index, int length);
};

#endif // BYTEARRAY_H
