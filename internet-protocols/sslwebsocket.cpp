#include "sslwebsocket.h"
#include "classes/p12fileextractor.h"
#include "classes/messageexception.h"

SslWebSocket::SslWebSocket() : InternetProtocol()
{
    this->socket = new QWebSocket();

    QObject::connect(this->socket, SIGNAL(binaryMessageReceived(QByteArray)),             this, SLOT(receivedByteArray(QByteArray)));
    QObject::connect(this->socket, SIGNAL(textMessageReceived(QString)),                  this, SLOT(receivedString(QString)));
    QObject::connect(this->socket, SIGNAL(disconnected()),                                this, SLOT(disconnected()));
    QObject::connect(this->socket, SIGNAL(error(QAbstractSocket::SocketError)),           this, SLOT(error(QAbstractSocket::SocketError)));
    QObject::connect(this->socket, SIGNAL(stateChanged(QAbstractSocket::SocketState)),    this, SLOT(stateDidChanged(QAbstractSocket::SocketState)));
}

SslWebSocket::SslWebSocket(QString withHost, int andPort) : SslWebSocket()
{
    this->setHost(withHost);
    this->setPort(andPort);
}

bool SslWebSocket::setCertificate(QString pem, QString pass)
{
    try {

        QByteArray * keyData = getKeyFromString(pem.toUtf8());
        QByteArray keyPass = pass.toUtf8();
        QSsl::KeyAlgorithm algo;
        QString str = keyData->data();
        if(str.startsWith(BEGINKEYSTRING))
            algo = QSsl::KeyAlgorithm::Dsa;
         else
            algo = QSsl::KeyAlgorithm::Rsa;
        QSslKey sslKey = QSslKey(*keyData, QSsl::KeyAlgorithm::Rsa, QSsl::EncodingFormat::Pem, QSsl::KeyType::PrivateKey, keyPass);
        QByteArray certData = pem.toUtf8();
        QList<QSslCertificate> certs = QSslCertificate::fromData(certData,QSsl::Pem);

        QSslConfiguration conf = QSslConfiguration();
        conf.setLocalCertificateChain(certs);
        conf.setPrivateKey(sslKey);
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

    QString urlString = "wss://" + this->getHost() + ":" + QString::number(this->getPort())+"/ws";
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
        QString message = QString(data);
        qint64 bytes = this->socket->sendTextMessage(message);
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
    printf("RECEIVED BYTE ARRAY\n");
    if (array.size() > 0) {
        emit didReceiveMessage(this, array);
    }
}

void SslWebSocket::receivedString(QString string)
{
    printf("RECEIVE STRING : %s\n", string.toUtf8().data());
    if (string.length() > 0) {
        emit didReceiveMessage(this, string.toUtf8());
    }
}

void SslWebSocket::readyRead()
{
    printf("REDY TO READ\n");
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
