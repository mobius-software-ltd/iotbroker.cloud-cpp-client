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
