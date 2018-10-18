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

QString InternetProtocol::socketErrorToString(QAbstractSocket::SocketError error)
{
    switch (error) {
        case QAbstractSocket::ConnectionRefusedError:
            return QString("connection refused error");
        case QAbstractSocket::RemoteHostClosedError:
            return QString("remote host closed error");
        case QAbstractSocket::HostNotFoundError:
            return QString("host not found error");
        case QAbstractSocket::SocketAccessError:
            return QString("socket access error");
        case QAbstractSocket::SocketResourceError:
            return QString("socket resource error");
        case QAbstractSocket::SocketTimeoutError:
            return QString("socket timeout error");
        case QAbstractSocket::DatagramTooLargeError:
            return QString("datagram too large error");
        case QAbstractSocket::NetworkError:
            return QString("network error");
        case QAbstractSocket::AddressInUseError:
            return QString("address in use error");
        case QAbstractSocket::SocketAddressNotAvailableError:
            return QString("socket address not available error");
        case QAbstractSocket::UnsupportedSocketOperationError:
            return QString("unsupported socket operation error");
        case QAbstractSocket::UnfinishedSocketOperationError:
            return QString("unfinished socket operation error");
        case QAbstractSocket::ProxyAuthenticationRequiredError:
            return QString("proxy authentication required error");
        case QAbstractSocket::SslHandshakeFailedError:
            return QString("ssl handshake failed error");
        case QAbstractSocket::ProxyConnectionRefusedError:
            return QString("proxy connection refused error");
        case QAbstractSocket::ProxyConnectionClosedError:
            return QString("proxy connection closed error");
        case QAbstractSocket::ProxyConnectionTimeoutError:
            return QString("proxy connection timeout error");
        case QAbstractSocket::ProxyNotFoundError:
            return QString("proxy not found error");
        case QAbstractSocket::ProxyProtocolError:
            return QString("proxy protocol error");
        case QAbstractSocket::OperationError:
            return QString("operation error");
        case QAbstractSocket::SslInternalError:
            return QString("ssl internal error");
        case QAbstractSocket::SslInvalidUserDataError:
            return QString("ssl invalid user data error");
        case QAbstractSocket::TemporaryError:
            return QString("temporary error");
        case QAbstractSocket::UnknownSocketError:
            return QString("unknown socket error");
    }

}

// SLOTS

void InternetProtocol::connected()
{

}

void InternetProtocol::received(char*)
{

}

void InternetProtocol::errorString(char *)
{

}

void InternetProtocol::receivedByteArray(QByteArray)
{

}

void InternetProtocol::receivedString(QString)
{

}


