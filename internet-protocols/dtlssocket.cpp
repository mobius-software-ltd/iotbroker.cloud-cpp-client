#include "dtlssocket.h"
#include "classes/p12fileextractor.h"
#include "classes/messageexception.h"

DtlsSocket::DtlsSocket() : InternetProtocol()
{
    this->socket = new Dtls();

    QObject::connect(socket, SIGNAL(didConnect()), this, SLOT(connected()));
    QObject::connect(socket, SIGNAL(didDisconnect()), this, SLOT(disconnected()));
    QObject::connect(socket, SIGNAL(received(char*, int)), this, SLOT(received(char*, int)));
    QObject::connect(socket, SIGNAL(error(char*)), this, SLOT(errorString(char*)));
}

DtlsSocket::DtlsSocket(QString withHost, int port) : DtlsSocket()
{
    char* dest;
    size_t destination_size = withHost.size()+1;
    dest = new char[destination_size];
    memcpy(dest, withHost.toLocal8Bit(),destination_size);

    this->socket->setHost(dest, port);
    this->setHost(withHost);
    this->setPort(port);
}

bool DtlsSocket::setCertificate(QString pem, QString pass)
{
    if(pem.isNull() || pem.isEmpty())
        return true;
    try {
        QByteArray * keyData = getKeyFromString(pem.toUtf8());
        pem.remove(*keyData,Qt::CaseSensitive);
        QByteArray keyPass = pass.toUtf8();
        QString str = keyData->data();
        QSsl::KeyAlgorithm algo;
        if(str.startsWith(BEGINDSAKEYSTRING))
            algo = QSsl::KeyAlgorithm::Dsa;
         else
            algo = QSsl::KeyAlgorithm::Rsa;
        QSslKey sslKey = QSslKey(*keyData, algo, QSsl::EncodingFormat::Pem, QSsl::KeyType::PrivateKey, keyPass);
        QString s_data = QString::fromStdString(sslKey.toPem().toStdString());
        this->socket->setCertsAndKey(pem,s_data);
    } catch (MessageException e) {
        emit didFailWithError(this, e.getMessage());
        return false;
    }
    return true;
}

void DtlsSocket::start()
{
//    if (this->getState() == IP_CONNECTION_OPEN) {
//        this->socket->stop();
//        this->future.cancel();
//    }
    this->future = QtConcurrent::run(this->socket, &Dtls::start);
}

void DtlsSocket::stop()
{
    if (this->getState() != IP_CONNECTION_CLOSED) {
        this->socket->stop();
        //cancel does not work on future
        //this->future.cancel();
    }
}

bool DtlsSocket::send(QByteArray data)
{
    if (this->getState() == IP_CONNECTION_OPEN) {
        this->socket->send(data);
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

void DtlsSocket::received(char *message, int size)
{
    QByteArray array = QByteArray(message, size);
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
    qInfo("set state closed");
    this->setState(IP_CONNECTION_CLOSED);
    emit connectionDidStop(this);
}

void DtlsSocket::error(QAbstractSocket::SocketError error)
{

}

void DtlsSocket::stateDidChanged(QAbstractSocket::SocketState state)
{

}

