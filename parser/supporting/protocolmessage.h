#ifndef PROTOCOLMESSAGE_H
#define PROTOCOLMESSAGE_H

enum Protocol
{
    MQTT_PROTOCOL = 1,
    MQTT_SN_PROTOCOL = 2,
    COAP_PROTOCOL = 3,
    AMQP_PROTOCOL = 4
};

/**
 * @brief The ProtocolMessage class
 */

class ProtocolMessage
{

public:
    virtual Protocol getProtocol() = 0;

};

#endif // PROTOCOLMESSAGE_H
