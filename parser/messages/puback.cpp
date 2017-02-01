#include "puback.h"

Puback::Puback()
{
    this->packetID = 0;
}

Puback::Puback(int packetID)
{
    this->packetID = packetID;
}

int Puback::getPacketID()
{
    return this->packetID;
}

void Puback::setPacketID(int packetID)
{
    this->packetID = packetID;
}

int Puback::getLength()
{
    return 2;
}

MessageType Puback::getType()
{
    return PUBACK;
}
