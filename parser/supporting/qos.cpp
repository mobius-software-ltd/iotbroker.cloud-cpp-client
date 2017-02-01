#include "qos.h"

QoS::QoS( )
{

}

QoS::QoS(unsigned char value)
{
    this->value = value;
}

int QoS::getValue()
{
    return this->value;
}

void QoS::setValue(unsigned char value)
{
    this->value = value;
}

QoS *QoS::calculate(QoS *subscriberQos, QoS *publisherQos)
{
    if (subscriberQos->getValue() == publisherQos->getValue()) {
        return subscriberQos;
    }

    if (subscriberQos->getValue() > publisherQos->getValue()) {
        return publisherQos;
    } else {
        return subscriberQos;
    }
}

bool QoS::isValid()
{
    if (this->value == AT_MOST_ONCE || this->value == AT_LEAST_ONCE || this->value == EXACTLY_ONCE) {
        return true;
    }
    return false;
}
