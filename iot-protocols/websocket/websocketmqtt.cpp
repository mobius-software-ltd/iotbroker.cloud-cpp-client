#include "websocketmqtt.h"
#include "internet-protocols/websocket.h"
#include "internet-protocols/sslwebsocket.h"
#include "iot-protocols/mqtt/classes/mqjsonparser.h"

WebsocketMQTT::WebsocketMQTT(AccountEntity account) : MQTT(account)
{
    this->timers = new TimersMap(this);
    this->keepAlive = account.keepAlive;

    this->publishPackets = new QMap<int, Message *>();

    this->messageParser = new MessagesParser(this);

    if (account.isSecure) {
        this->internetProtocol = new SslWebSocket(account.serverHost.get().toString(), account.port.get().toInt());
    } else {
        this->internetProtocol = new WebSocket(account.serverHost.get().toString(), account.port.get().toInt());
    }

    QObject::connect(this->internetProtocol, SIGNAL(connectionDidStart(InternetProtocol*)),             this, SLOT(connectionDidStart(InternetProtocol*)));
    QObject::connect(this->internetProtocol, SIGNAL(connectionDidStop(InternetProtocol*)),              this, SLOT(connectionDidStop(InternetProtocol*)));
    QObject::connect(this->internetProtocol, SIGNAL(didReceiveMessage(InternetProtocol*,QByteArray)),   this, SLOT(didReceiveMessage(InternetProtocol*,QByteArray)));
    QObject::connect(this->internetProtocol, SIGNAL(didFailWithError(InternetProtocol*,QString)),       this, SLOT(didFailWithError(InternetProtocol*,QString)));
}

bool WebsocketMQTT::send(Message *message)
{
    if (this->internetProtocol->getState() == IP_CONNECTION_OPEN) {
        QByteArray data = MQJsonParser::json(message);
        return this->internetProtocol->send(data);
    }
    return false;
}

void WebsocketMQTT::didReceiveMessage(InternetProtocol *protocol, QByteArray data)
{
    Q_UNUSED(protocol);
    WebsocketMQTT* parent = (WebsocketMQTT*)this->parent();

    Message *message = MQJsonParser::message(data);
    parent->didReceiveMessage(this->internetProtocol, this->messageParser->encodeMessage(message));
}
