#include "topic.h"

Topic::Topic()
{

}

Topic::Topic(QString name, QoS *qos)
{
    this->name = name;
    this->qos = new QoS(qos->getValue());
}

QString Topic::toString()
{
    return this->name + ":" + QString::number(this->qos->getValue());
}

QString Topic::getName() const
{
    return this->name;
}

void Topic::setName(QString name)
{
    this->name = name;
}

QoS *Topic::getQoS() const
{
    return this->qos;
}

void Topic::setQoS(QoS *qos)
{
    this->qos = qos;
}

int Topic::length()
{
    return this->name.length();
}
