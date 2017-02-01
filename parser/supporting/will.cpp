#include "will.h"

Will::Will()
{

}

Will::Will(Topic *topic, QByteArray content, bool retain)
{
    this->topic = topic;
    this->content = content;
    this->retain = retain;
}

int Will::retrieveLength()
{
    return this->topic->length() + this->content.size() + 4;
}

Topic *Will::getTopic()
{
    return this->topic;
}

void Will::setTopic(Topic *topic)
{
    this->topic = topic;
}

QByteArray Will::getContent()
{
    return this->content;
}

void Will::setContent(QByteArray content)
{
    this->content = content;
}

bool Will::isRetain()
{
    return this->retain;
}

void Will::setRetain(bool retain)
{
    this->retain = retain;
}

bool Will::isValid()
{
    return this->topic != NULL && this->topic->length() > 0 && !this->content.isEmpty() && this->topic->getQoS() != NULL;
}
