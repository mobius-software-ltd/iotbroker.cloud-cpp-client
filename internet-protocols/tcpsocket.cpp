#include "tcpsocket.h"

TCPSocket::TCPSocket() : InternetProtocol()
{
    this->socket = new QTcpSocket(this);

    //QObject::connect(socket, SIGNAL(aboutToClose()),                                             this, SLOT(aboutToCloseSlot()));

    QObject::connect(socket, SIGNAL(channelReadyRead(int)),                         this, SLOT(readyRead()));
    QObject::connect(socket, SIGNAL(disconnected()),                                this, SLOT(disconnected()));
    QObject::connect(socket, SIGNAL(error(QAbstractSocket::SocketError)),           this, SLOT(error(QAbstractSocket::SocketError)));
    QObject::connect(socket, SIGNAL(stateChanged(QAbstractSocket::SocketState)),    this, SLOT(stateDidChanged(QAbstractSocket::SocketState)));
}

TCPSocket::TCPSocket(QString withHost, int andPort) : TCPSocket()
{
    this->setHost(withHost);
    this->setPort(andPort);
}

void TCPSocket::start()
{
    if (this->getState() == IP_CONNECTION_OPEN) {
        this->socket->disconnectFromHost();
    }
    this->socket->connectToHost(this->getHost(), this->getPort());
}

void TCPSocket::stop()
{
    if (this->getState() != IP_CONNECTION_CLOSED) {
        this->socket->disconnectFromHost();
    }
}

bool TCPSocket::send(QByteArray data)
{
    if (this->getState() == IP_CONNECTION_OPEN) {
        qint64 bytes = this->socket->write(data.data(), data.size());
        if (bytes == -1) {
            return false;
        }
        return true;
    }
    return false;
}

// SLOTS

void TCPSocket::readyRead()
{

    QByteArray byteArray = QByteArray();

    while (socket->bytesAvailable()) {
       byteArray.append(socket->readAll());
    }

    if (byteArray.size() > 0) {
        emit didReceiveMessage(this, byteArray);
    }
}

void TCPSocket::disconnected()
{
    emit connectionDidStop(this);
}

void TCPSocket::error(QAbstractSocket::SocketError error)
{
    emit didFailWithError(this, error);
}

void TCPSocket::stateDidChanged(QAbstractSocket::SocketState state)
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
