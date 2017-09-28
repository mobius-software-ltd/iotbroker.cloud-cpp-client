#include "udpsocket.h"

UDPSocket::UDPSocket() : InternetProtocol()
{
    this->socket = new QUdpSocket(this);

    //QObject::connect(socket, SIGNAL(aboutToClose()),                                             this, SLOT(aboutToCloseSlot()));

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

void UDPSocket::start()
{
    this->socket->bind(QHostAddress(this->getHost()), 0);
}

void UDPSocket::stop()
{
    this->socket->close();
}

bool UDPSocket::send(QByteArray data)
{
    qint64 bytes = this->socket->writeDatagram(data, QHostAddress(this->getHost()), this->getPort());
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

    emit didReceiveMessage(this, data);
}

void UDPSocket::disconnected()
{
    emit connectionDidStop(this);
}

void UDPSocket::error(QAbstractSocket::SocketError error)
{
    emit didFailWithError(this, error);
}

void UDPSocket::stateDidChanged(QAbstractSocket::SocketState state)
{
    switch (state) {
        case QAbstractSocket::UnconnectedState: this->setState(IP_CONNECTION_CLOSED);   break;
        case QAbstractSocket::ConnectingState:  this->setState(IP_CONNECTION_OPENING);  break;
        case QAbstractSocket::ConnectedState:   this->setState(IP_CONNECTION_OPEN);     break;
        case QAbstractSocket::ClosingState:     this->setState(IP_CONNECTION_CLOSING);  break;
        default: break;
    }

    if (state == QAbstractSocket::ConnectedState) {
        emit connectionDidStart(this);
    }
}
