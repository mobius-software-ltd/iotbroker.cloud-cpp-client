#include "convertor.h"
#include "classes/bytearray.h"

Convertor::Convertor()
{

}

long Convertor::bytesToNumber(QByteArray array)
{
    ByteArray buf = ByteArray(array);
    if (array.size() == 1) {
        return (unsigned char)buf.readChar();
    } else if (array.size() == 2) {
        return buf.readShort();
    } else if (array.size() <= 4) {
        return buf.readInt();
    } else {
        return buf.readLong();
    }
    return buf.readInt();
}

QByteArray Convertor::numberToBytes(long num)
{
    ByteArray buf = ByteArray();

    if (num == 0) {

    } else if (num < 255) {
        buf.writeChar((char)num);
    } else if (num <= 65535) {
        buf.writeShort((short)num);
    } else if (num <= 16777215) {
        buf.writeInt((int)num);
    } else {
        buf.writeLong(num);
    }

    return buf.getByteArray();
}

int Convertor::length(long num)
{
    if (num <= 255) {
        return 1;
    } else if (num <= 65535) {
        return 3;
    } else if (num <= 16777215) {
        return 4;
    } else {
        return 8;
    }
    return 0;
}
