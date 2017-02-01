#include "lengthdetails.h"

LengthDetails::LengthDetails()
{

}

LengthDetails::LengthDetails(int length, int size)
{
    this->length = length;
    this->size = size;
}

int LengthDetails::getLength()
{
    return this->length;
}

int LengthDetails::getSize()
{
    return this->size;
}

LengthDetails *LengthDetails::decode(ByteArray *byteArray)
{
    int length = 0;
    int multiplier = 1;
    int byteUsed = 0;
    char encodedByte = 0;

    do {
        if (!(multiplier > 128 * 128 * 128)) {
            encodedByte = byteArray->readChar();
            length += (encodedByte & 0x7f) * multiplier;
            multiplier *= 128;
            byteUsed++;
        }
    } while ((encodedByte & 128) != 0);

    return new LengthDetails(length, byteUsed);
}
