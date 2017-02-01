#include "publish.h"

Publish::Publish()
{
    this->packetID = 0;
}

Publish::Publish(int packetID)
{
    this->packetID = packetID;
}

Publish::Publish(Topic *topic, QByteArray content, bool retain, bool dup)
{
    this->packetID = 0;
    this->topic = topic;
    this->content = content;
    this->retain = retain;
    this->dup = dup;
}

Publish::Publish(int packetID, Topic *topic, QByteArray content, bool retain, bool dup)
{
    this->packetID = packetID;
    this->topic = topic;
    this->content = content;
    this->retain = retain;
    this->dup = dup;
}

int Publish::getPacketID()
{
    return this->packetID;
}

void Publish::setPacketID(int packetID)
{
    this->packetID = packetID;
}

int Publish::getLength()
{
    int length = 0;
    length += this->packetID != 0 ? 2 : 0;
    length += this->topic->length() + 2;
    length += this->content.length();
    return length;
}

MessageType Publish::getType()
{
    return PUBLISH;
}

Topic *Publish::getTopic()
{
    return this->topic;
}

void Publish::setTopic(Topic *topic)
{
    this->topic = topic;
}

QByteArray Publish::getContent()
{
    return this->content;
}

void Publish::setContent(QByteArray content)
{
    this->content = content;
}

bool Publish::isRetain()
{
    return this->retain;
}

void Publish::setRetain(bool retain)
{
    this->retain = retain;
}

bool Publish::isDup()
{
    return this->dup;
}

void Publish::setDup(bool dup)
{
    this->dup = dup;
}
