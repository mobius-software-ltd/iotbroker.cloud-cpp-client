#include "dtlssocket.h"
#include "classes/p12fileextractor.h"
#include "classes/messageexception.h"

DtlsSocket::DtlsSocket() : InternetProtocol()
{
    this->socket = new Dtls();

    QObject::connect(socket, SIGNAL(didConnect()), this, SLOT(connected()));
    QObject::connect(socket, SIGNAL(didDisconnect()), this, SLOT(disconnected()));
    QObject::connect(socket, SIGNAL(received(char*)), this, SLOT(received(char*)));
    QObject::connect(socket, SIGNAL(error(char*)), this, SLOT(errorString(char*)));
}

DtlsSocket::DtlsSocket(QString withHost, int port) : DtlsSocket()
{
    char *src = withHost.toUtf8().data();
    char *dis = (char *)malloc(sizeof(char) * withHost.size());

    size_t destination_size = withHost.size();

    strncpy(dis, src, destination_size);
    dis[destination_size] = '\0';

    this->socket->setHost(dis, port);
    this->setHost(withHost);
    this->setPort(port);
}

bool DtlsSocket::setCertificate(QString path, QString pass)
{
    try {
        P12FileExtractor extractor = P12FileExtractor(path.toUtf8().data(), pass.toUtf8().data());
        this->socket->setCerts(extractor.getCaCertificate(), extractor.getCertificate());
    } catch (MessageException e) {
        emit didFailWithError(this, e.getMessage());
        return false;
    }
    return true;
}

void DtlsSocket::start()
{
    if (this->getState() == IP_CONNECTION_OPEN) {
        this->socket->stop();
        this->future.cancel();
    }
    this->future = QtConcurrent::run(this->socket, &Dtls::start);
}

void DtlsSocket::stop()
{
    if (this->getState() != IP_CONNECTION_CLOSED) {
        this->socket->stop();
        this->future.cancel();
    }
}

bool DtlsSocket::send(QByteArray data)
{
    if (this->getState() == IP_CONNECTION_OPEN) {
        this->socket->send(data.data());
        return true;
    }
    return false;
}

// SLOTS

void DtlsSocket::connected()
{
    this->setState(IP_CONNECTION_OPEN);
    emit connectionDidStart(this);
}

void DtlsSocket::received(char *message)
{
    QByteArray array = QByteArray(message);
    emit didReceiveMessage(this, array);
}

void DtlsSocket::errorString(char *message)
{
    emit didFailWithError(this, QString(message));
}

void DtlsSocket::readyRead()
{

}

void DtlsSocket::disconnected()
{
    this->setState(IP_CONNECTION_CLOSED);
    emit connectionDidStop(this);
}

void DtlsSocket::error(QAbstractSocket::SocketError error)
{

}

void DtlsSocket::stateDidChanged(QAbstractSocket::SocketState state)
{

}

