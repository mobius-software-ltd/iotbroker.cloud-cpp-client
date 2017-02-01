#include "connect.h"

Connect::Connect()
{

}

Connect::Connect(QString username, QString password, QString clientID, bool cleanSession, int keepAlive, Will *will)
{
    this->username = username;
    this->password = password;
    this->clientID = clientID;
    this->cleanSession = cleanSession;
    this->keepAlive = keepAlive;
    this->will = will;
    this->protocolLevel = 4;
}

int Connect::getLength()
{
    int length = 10;
    length += this->clientID.length() + 2;
    length += isWillFlag() ? this->will->retrieveLength() : 0;
    length += this->username != NULL ? this->username.length() + 2 : 0;
    length += this->password != NULL ? this->password.length() + 2 : 0;
    return length;
}

MessageType Connect::getType()
{
    return CONNECT;
}

QString Connect::getProtocolName()
{
    QString name;

    switch (this->getProtocol()) {
        case MQTT_PROTOCOL:      name = "MQTT";      break;
        case MQTT_SN_PROTOCOL:   name = "MQTT_SN";   break;
        case COAP_PROTOCOL:      name = "COAP";      break;
        case AMQP_PROTOCOL:      name = "AMQP";      break;
    }
    return name;
}

int Connect::getProtocolLevel()
{
    return this->protocolLevel;
}

void Connect::setProtocolLevel(int protocolLevel)
{
    this->protocolLevel = protocolLevel;
}

bool Connect::isCleanSession()
{
    return this->cleanSession;
}

void Connect::setCleanSession(bool cleanSession)
{
    this->cleanSession = cleanSession;
}

bool Connect::isWillFlag()
{
    return this->will != NULL;
}

Will *Connect::getWill()
{
    return this->will;
}

void Connect::setWill(Will *will)
{
    this->will = will;
}

int Connect::getKeepAlive()
{
    return this->keepAlive;
}

void Connect::setKeepAlive(bool keepAlive)
{
    this->keepAlive = keepAlive;
}

QString Connect::getClientID()
{
    return this->clientID;
}

void Connect::setClientID(QString clientID)
{
    this->clientID = clientID;
}

bool Connect::isUsernameFlag()
{
    return this->username != NULL;
}

QString Connect::getUsername()
{
    return this->username;
}

void Connect::setUsername(QString username)
{
    this->username = username;
}

bool Connect::isPasswordFlag()
{
    return this->password != NULL;
}

QString Connect::getPassword()
{
    return this->password;
}

void Connect::setPassword(QString password)
{
    this->password = password;
}
