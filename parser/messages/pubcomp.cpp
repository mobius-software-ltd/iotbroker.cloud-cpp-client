#include "pubcomp.h"

Pubcomp::Pubcomp()
{
    this->packetID = 0;
}

Pubcomp::Pubcomp(int packetID)
{
    this->packetID = packetID;
}

int Pubcomp::getPacketID()
{
    return this->packetID;
}

void Pubcomp::setPacketID(int packetID)
{
    this->packetID = packetID;
}

int Pubcomp::getLength()
{
    return 2;
}

MessageType Pubcomp::getType()
{
    return PUBCOMP;
}
