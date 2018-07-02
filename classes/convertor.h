#ifndef CONVERTOR_H
#define CONVERTOR_H

#include <QByteArray>

class Convertor
{
public:
    Convertor();

    static long bytesToNumber(QByteArray array);
    static QByteArray numberToBytes(long num);
    static int length(long num);

};

#endif // CONVERTOR_H
