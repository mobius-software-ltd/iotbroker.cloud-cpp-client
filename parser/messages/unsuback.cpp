#include "unsuback.h"

Unsuback::Unsuback()
{
    this->packetID = 0;
}

Unsuback::Unsuback(int packetID)
{
    this->packetID = packetID;
}

int Unsuback::getPacketID()
{
    return this->packetID;
}

void Unsuback::setPacketID(int packetID)
{
    this->packetID = packetID;
}

int Unsuback::getLength()
{
    return 2;
}

MessageType Unsuback::getType()
{
    return UNSUBACK;
}
