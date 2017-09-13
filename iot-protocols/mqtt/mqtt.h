#ifndef MQTT_H
#define MQTT_H

#include "iot-protocols/iotprotocol.h"

class MQTT : public IotProtocol
{

private:

    int keepAlive;
    Connect *connect;
    QMap<int, Message *> *exactlyOncePublishPackets;

public:
    MQTT(AccountEntity account);

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
    void didFailWithError(InternetProtocol *protocol, QAbstractSocket::SocketError error);

};

#endif // MQTT_H
