#ifndef QOS_H
#define QOS_H

enum QoSType : unsigned char
{
    AT_MOST_ONCE = 0,
    AT_LEAST_ONCE = 1,
    EXACTLY_ONCE = 2
};

/**
 * @brief The QoS class
 */

class QoS
{

private:
    unsigned char value;

public:
    QoS();
    QoS(unsigned char value);

    int getValue();
    void setValue(unsigned char value);

    static QoS *calculate(QoS *subscriberQos, QoS *publisherQos);

    bool isValid();
};

#endif // QOS_H
