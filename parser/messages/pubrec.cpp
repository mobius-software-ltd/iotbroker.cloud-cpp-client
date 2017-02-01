#include "pubrec.h"

Pubrec::Pubrec()
{
    this->packetID = 0;
}

Pubrec::Pubrec(int packetID)
{
    this->packetID = packetID;
}

int Pubrec::getPacketID()
{
    return this->packetID;
}

void Pubrec::setPacketID(int packetID)
{
    this->packetID = packetID;
}

int Pubrec::getLength()
{
    return 2;
}

MessageType Pubrec::getType()
{
    return PUBREC;
}
