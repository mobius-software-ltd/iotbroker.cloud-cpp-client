#ifndef LENGTHDETAILS_H
#define LENGTHDETAILS_H

#include <QBuffer>
#include <parser/supporting/bytearray.h>

/**
 * @brief The LengthDetails class
 */

class LengthDetails
{

private:
    int length;
    int size;

public:
    LengthDetails();
    LengthDetails(int length, int size);

    int getLength();
    int getSize();

    static LengthDetails *decode(ByteArray *byteArray);
};

#endif // LENGTHDETAILS_H
