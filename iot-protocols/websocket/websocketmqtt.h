#ifndef WEBSOCKETMQTT_H
#define WEBSOCKETMQTT_H

#include "iot-protocols/iotprotocol.h"
#include "iot-protocols/mqtt/parser/messagesparser.h"
#include "iot-protocols/mqtt/mqtt.h"

class WebsocketMQTT : public MQTT
{

public:
    WebsocketMQTT(AccountEntity account);
    bool send(Message *message);
    void didReceiveMessage(InternetProtocol *protocol, QByteArray data);
};

#endif // WEBSOCKETMQTT_H
