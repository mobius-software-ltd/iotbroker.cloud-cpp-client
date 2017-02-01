#include "subscribe.h"

Subscribe::Subscribe()
{
    this->packetID = 0;
}

Subscribe::Subscribe(int packetID)
{
    this->packetID = packetID;
}

Subscribe::Subscribe(QList<Topic> *topics)
{
    this->packetID = 0;
    this->topics = topics;
}

Subscribe::Subscribe(int packetID, QList<Topic> *topics)
{
    this->packetID = packetID;
    this->topics = topics;
}

int Subscribe::getPacketID()
{
    return this->packetID;
}

void Subscribe::setPacketID(int packetID)
{
    this->packetID = packetID;
}

int Subscribe::getLength()
{
    int length = 0;
    length += this->packetID != 0 ? 2 : 0;
    for (int i = 0; i < this->topics->size(); i++) {
        Topic item = this->topics->at(i);
        length += item.getName().length() + 3;
    }
    return length;
}

MessageType Subscribe::getType()
{
    return SUBSCRIBE;
}

QList<Topic> *Subscribe::getTopics()
{
    return this->topics;
}

void Subscribe::setTopics(QList<Topic> *topics)
{
    this->topics = topics;
}
