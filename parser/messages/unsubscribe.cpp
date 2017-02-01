#include "unsubscribe.h"

Unsubscribe::Unsubscribe()
{
    this->packetID = 0;
}

Unsubscribe::Unsubscribe(QList<QString> *topics)
{
    this->packetID = 0;
    this->topics = topics;
}

Unsubscribe::Unsubscribe(int packetID, QList<QString> *topics)
{
    this->packetID = packetID;
    this->topics = topics;
}

int Unsubscribe::getPacketID()
{
    return this->packetID;
}

void Unsubscribe::setPacketID(int packetID)
{
    this->packetID = packetID;
}

int Unsubscribe::getLength()
{
    int length = 2;
    for (int i = 0; i < this->topics->size(); i++) {
        QString item = this->topics->at(i);
        length += item.length() + 2;
    }
    return length;
}

MessageType Unsubscribe::getType()
{
    return UNSUBSCRIBE;
}

QList<QString> *Unsubscribe::getTopics()
{
    return topics;
}

void Unsubscribe::setTopics(QList<QString> *topics)
{
    this->topics = topics;
}
