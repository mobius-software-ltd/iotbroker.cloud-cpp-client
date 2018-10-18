#ifndef WEBSOCKETMQTT_H
#define WEBSOCKETMQTT_H

#include "iot-protocols/iotprotocol.h"
#include "iot-protocols/mqtt/parser/messagesparser.h"
#include "iot-protocols/mqtt/mqtt.h"

class WebsocketMQTT : public IotProtocol
{

protected:

    int keepAlive;
    Connect *connect;
    QMap<int, Message *> *publishPackets;
    MessagesParser *messageParser;

public:
    WebsocketMQTT(AccountEntity account);

    virtual bool send(Message *message);

    virtual void goConnect();
    virtual void publish(MessageEntity message);
    virtual void subscribeTo(TopicEntity topic);
    virtual void unsubscribeFrom(TopicEntity topic);
    virtual void pingreq();
    virtual void disconnectWith(int duration);

    virtual Message *getPingreqMessage();

    virtual void timeoutMethod();

public slots:

    void connectionDidStart(InternetProtocol *protocol);
    void connectionDidStop(InternetProtocol *protocol);
    void didReceiveMessage(InternetProtocol *protocol, QByteArray data);
    void didFailWithError(InternetProtocol *protocol, QString error);

    void parseFailWithError(QString *error);
};

#endif // WEBSOCKETMQTT_H
