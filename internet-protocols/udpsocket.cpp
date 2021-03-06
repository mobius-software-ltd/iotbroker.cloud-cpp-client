#include "udpsocket.h"

UDPSocket::UDPSocket() : InternetProtocol()
{
    this->socket = new QUdpSocket(this);

    QObject::connect(this->socket, SIGNAL(readyRead()),                                  this, SLOT(readyRead()));
    QObject::connect(this->socket, SIGNAL(disconnected()),                               this, SLOT(disconnected()));
    QObject::connect(this->socket, SIGNAL(error(QAbstractSocket::SocketError)),          this, SLOT(error(QAbstractSocket::SocketError)));
    QObject::connect(this->socket, SIGNAL(stateChanged(QAbstractSocket::SocketState)),   this, SLOT(stateDidChanged(QAbstractSocket::SocketState)));
}

UDPSocket::UDPSocket(QString withHost, int andPort) : UDPSocket()
{
    this->setHost(withHost);
    this->setPort(andPort);
}

bool UDPSocket::setCertificate(QString path, QString pass)
{
    Q_UNUSED(path);
    Q_UNUSED(pass);
    return true;
}

void UDPSocket::start()
{
    this->socket->connectToHost(this->getHost(), this->getPort(), QIODevice::ReadWrite);
}

void UDPSocket::stop()
{
    this->socket->close();
}

bool UDPSocket::send(QByteArray data)
{
    qint64 bytes =this->socket->write(data);
    if (bytes == -1) {
        return false;
    }
    return true;
}

// SLOTS

void UDPSocket::readyRead()
{
    QByteArray data;
    data.resize(socket->pendingDatagramSize());

    QHostAddress senderHost;
    quint16 senderPort;
    socket->readDatagram(data.data(), data.size(), &senderHost, &senderPort);

    if(data.size()>0)
        emit didReceiveMessage(this, data);
    else
         this->socket->close();
}

void UDPSocket::disconnected()
{

    emit connectionDidStop(this);
}

void UDPSocket::error(QAbstractSocket::SocketError error)
{
    QString stringError = InternetProtocol::socketErrorToString(error);
    emit didFailWithError(this, stringError);
}

void UDPSocket::stateDidChanged(QAbstractSocket::SocketState state)
{
    switch (state) {
        case QAbstractSocket::UnconnectedState: this->setState(IP_CONNECTION_CLOSED);   break;
        case QAbstractSocket::ConnectingState:  this->setState(IP_CONNECTION_OPENING);  break;
        case QAbstractSocket::ConnectedState:   this->setState(IP_CONNECTION_OPEN);     break;
        case QAbstractSocket::BoundState:       this->setState(IP_CONNECTION_OPEN);     break;
        case QAbstractSocket::ClosingState:     this->setState(IP_CONNECTION_CLOSING);  break;
        default: break;
    }

    if (state == QAbstractSocket::ConnectedState) {
        emit connectionDidStart(this);
    }
}
