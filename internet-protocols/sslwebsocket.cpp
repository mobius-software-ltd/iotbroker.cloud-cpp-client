#include "sslwebsocket.h"
#include "classes/p12fileextractor.h"
#include "classes/messageexception.h"

SslWebSocket::SslWebSocket() : InternetProtocol()
{
    this->socket = new QWebSocket();

    QObject::connect(this->socket, SIGNAL(binaryMessageReceived(QByteArray)),             this, SLOT(receivedByteArray(QByteArray)));
    QObject::connect(this->socket, SIGNAL(disconnected()),                                this, SLOT(disconnected()));
    QObject::connect(this->socket, SIGNAL(error(QAbstractSocket::SocketError)),           this, SLOT(error(QAbstractSocket::SocketError)));
    QObject::connect(this->socket, SIGNAL(stateChanged(QAbstractSocket::SocketState)),    this, SLOT(stateDidChanged(QAbstractSocket::SocketState)));
}

SslWebSocket::SslWebSocket(QString withHost, int andPort) : SslWebSocket()
{
    this->setHost(withHost);
    this->setPort(andPort);
}

bool SslWebSocket::setCertificate(QString path, QString pass)
{
    try {
        P12FileExtractor extractor = P12FileExtractor(path.toUtf8().data(), pass.toUtf8().data());
        QByteArray certData = QByteArray(extractor.getCertificate(), strlen(extractor.getCertificate()));
        QSslCertificate certificate = QSslCertificate(certData);
        QSslConfiguration conf = QSslConfiguration();
        conf.setLocalCertificate(certificate);
        this->socket->setSslConfiguration(conf);
    } catch (MessageException e) {
        emit didFailWithError(this, e.getMessage());
        return false;
    }
    return true;
}

void SslWebSocket::start()
{
    if (this->getState() == IP_CONNECTION_OPEN) {
        this->socket->disconnected();
    }

    QString urlString = "wss://" + this->getHost() + ":" + QString::number(this->getPort());
    QUrl url = QUrl(urlString);
    this->socket->open(url);
}

void SslWebSocket::stop()
{
    if (this->getState() != IP_CONNECTION_CLOSED) {
        this->socket->disconnected();
    }
}

bool SslWebSocket::send(QByteArray data)
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

void SslWebSocket::receivedByteArray(QByteArray array)
{
    if (array.size() > 0) {
        emit didReceiveMessage(this, array);
    }
}

void SslWebSocket::readyRead()
{

}

void SslWebSocket::disconnected()
{
    emit connectionDidStop(this);
}

void SslWebSocket::error(QAbstractSocket::SocketError error)
{
    QString stringError = InternetProtocol::socketErrorToString(error);
    emit didFailWithError(this, stringError);
}

void SslWebSocket::stateDidChanged(QAbstractSocket::SocketState state)
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
