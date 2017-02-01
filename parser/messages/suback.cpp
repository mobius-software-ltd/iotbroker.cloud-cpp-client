#include "suback.h"

Suback::Suback()
{
    this->packetID = 0;
}

Suback::Suback(int packetID)
{
    this->packetID = packetID;
}

Suback::Suback(int packetID, QList<SubackCode> *returnCodes)
{
    this->packetID = packetID;
    this->returnCodes = returnCodes;
}

int Suback::getPacketID()
{
    return this->packetID;
}

void Suback::setPacketID(int packetID)
{
    this->packetID = packetID;
}

int Suback::getLength()
{
    return this->returnCodes->size() + 2;
}

MessageType Suback::getType()
{
    return SUBACK;
}

QList<SubackCode> *Suback::getReturnCodes()
{
    return this->returnCodes;
}

void Suback::setReturnCodes(QList<SubackCode> *returnCodes)
{
    this->returnCodes = returnCodes;
}

bool Suback::isValidCode(SubackCode code)
{
    if (code == ACCEPTED_QOS0 || code == ACCEPTED_QOS1 || code == ACCEPTED_QOS2) {
        return true;
    }
    return false;
}
