#include "pubrel.h"

Pubrel::Pubrel()
{
    this->packetID = 0;
}

Pubrel::Pubrel(int packetID)
{
    this->packetID = packetID;
}

int Pubrel::getPacketID()
{
    return this->packetID;
}

void Pubrel::setPacketID(int packetID)
{
    this->packetID = packetID;
}

int Pubrel::getLength()
{
    return 2;
}

MessageType Pubrel::getType()
{
    return PUBREL;
}
