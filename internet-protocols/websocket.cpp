#include "websocket.h"

WebSocket::WebSocket() : InternetProtocol()
{
    this->socket = new QWebSocket();

    QObject::connect(this->socket, SIGNAL(binaryMessageReceived(QByteArray)),             this, SLOT(receivedByteArray(QByteArray)));
    QObject::connect(this->socket, SIGNAL(disconnected()),                                this, SLOT(disconnected()));
    QObject::connect(this->socket, SIGNAL(error(QAbstractSocket::SocketError)),           this, SLOT(error(QAbstractSocket::SocketError)));
    QObject::connect(this->socket, SIGNAL(stateChanged(QAbstractSocket::SocketState)),    this, SLOT(stateDidChanged(QAbstractSocket::SocketState)));
}

WebSocket::WebSocket(QString withHost, int andPort) : WebSocket()
{
    this->setHost(withHost);
    this->setPort(andPort);
}

bool WebSocket::setCertificate(QString path, QString pass)
{
    Q_UNUSED(path);
    Q_UNUSED(pass);
    return true;
}

void WebSocket::start()
{
    if (this->getState() == IP_CONNECTION_OPEN) {
        this->socket->disconnected();
    }

    QString urlString = "ws://" + this->getHost() + ":" + QString::number(this->getPort());
    QUrl url = QUrl(urlString);
    this->socket->open(url);
}

void WebSocket::stop()
{
    if (this->getState() != IP_CONNECTION_CLOSED) {
        this->socket->disconnected();
    }
}

bool WebSocket::send(QByteArray data)
{
    if (this->getState() == IP_CONNECTION_OPEN) {
        qint64 bytes = this->socket->sendBinaryMessage(data);
        if (bytes == -1) {
            return false;
        }
        return true;
    }
    return false;
}

// SLOTS

void WebSocket::receivedByteArray(QByteArray array)
{
    if (array.size() > 0) {
        emit didReceiveMessage(this, array);
    }
}

void WebSocket::readyRead()
{

}

void WebSocket::disconnected()
{
    emit connectionDidStop(this);
}

void WebSocket::error(QAbstractSocket::SocketError error)
{
    QString stringError = InternetProtocol::socketErrorToString(error);
    emit didFailWithError(this, stringError);
}

void WebSocket::stateDidChanged(QAbstractSocket::SocketState state)
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

