#include "sslsocket.h"
#include "classes/p12fileextractor.h"
#include "classes/messageexception.h"

SslSocket::SslSocket() : InternetProtocol()
{
    this->socket = new QSslSocket(this);

    //QObject::connect(socket, SIGNAL(aboutToClose()),                              this, SLOT(aboutToCloseSlot()));

    QObject::connect(socket, SIGNAL(channelReadyRead(int)),                         this, SLOT(readyRead()));
    QObject::connect(socket, SIGNAL(disconnected()),                                this, SLOT(disconnected()));
    QObject::connect(socket, SIGNAL(error(QAbstractSocket::SocketError)),           this, SLOT(error(QAbstractSocket::SocketError)));
    QObject::connect(socket, SIGNAL(stateChanged(QAbstractSocket::SocketState)),    this, SLOT(stateDidChanged(QAbstractSocket::SocketState)));
}

SslSocket::SslSocket(QString withHost, int port) : SslSocket()
{
    this->setHost(withHost);
    this->setPort(port);
}

bool SslSocket::setCertificate(QString path, QString pass)
{
    try {
        P12FileExtractor extractor = P12FileExtractor(path.toUtf8().data(), pass.toUtf8().data());
        QByteArray certData = QByteArray(extractor.getCertificate(), strlen(extractor.getCertificate()));
        QSslCertificate certificate = QSslCertificate(certData);
        this->socket->setLocalCertificate(certificate);
    } catch (MessageException e) {
        emit didFailWithError(this, e.getMessage());
        return false;
    }
    return true;
}

void SslSocket::start()
{
    if (this->getState() == IP_CONNECTION_OPEN) {
        this->socket->disconnectFromHost();
    }
    this->socket->connectToHostEncrypted(this->getHost(), this->getPort());
}

void SslSocket::stop()
{
    if (this->getState() != IP_CONNECTION_CLOSED) {
        this->socket->disconnectFromHost();
    }
}

bool SslSocket::send(QByteArray data)
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

void SslSocket::readyRead()
{

    QByteArray byteArray = QByteArray();

    while (socket->bytesAvailable()) {
       byteArray.append(socket->readAll());
    }

    if (byteArray.size() > 0) {
        emit didReceiveMessage(this, byteArray);
    }
}

void SslSocket::disconnected()
{
    emit connectionDidStop(this);
}

void SslSocket::error(QAbstractSocket::SocketError error)
{
    QString stringError = InternetProtocol::socketErrorToString(error);
    emit didFailWithError(this, stringError);
}

void SslSocket::stateDidChanged(QAbstractSocket::SocketState state)
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