#ifndef MESSAGE_H
#define MESSAGE_H

#include <parser/supporting/protocolmessage.h>

enum MessageType
{
    CONNECT = 1,
    CONNACK = 2,
    PUBLISH = 3,
    PUBACK = 4,
    PUBREC = 5,
    PUBREL = 6,
    PUBCOMP = 7,
    SUBSCRIBE = 8,
    SUBACK = 9,
    UNSUBSCRIBE = 10,
    UNSUBACK = 11,
    PINGREQ = 12,
    PINGRESP = 13,
    DISCONNECT = 14
};

/**
 * @brief The Message class
 */

class Message : public ProtocolMessage
{
public:

    virtual int getLength() = 0;
    virtual MessageType getType() = 0;
    Protocol getProtocol()
    {
        return MQTT_PROTOCOL;
    }
};

#endif // MESSAGE_H
