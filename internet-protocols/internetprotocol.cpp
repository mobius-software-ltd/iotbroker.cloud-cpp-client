#include "internetprotocol.h"

InternetProtocol::InternetProtocol()
{
}

InternetProtocol::InternetProtocol(QString withHost, int andPort)
{
    this->host = withHost;
    this->port = andPort;
}

ConnectionState InternetProtocol::getState() const
{
    return this->state;
}

void InternetProtocol::setState(const ConnectionState &value)
{
    this->state = value;
}

QString InternetProtocol::getHost() const
{
    return this->host;
}

void InternetProtocol::setHost(const QString &value)
{
    this->host = value;
}

int InternetProtocol::getPort() const
{
    return this->port;
}

void InternetProtocol::setPort(int value)
{
    this->port = value;
}
