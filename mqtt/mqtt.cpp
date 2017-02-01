/**
 * Mobius Software LTD
 * Copyright 2015-2017, Mobius Software LTD
 *
 * This is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation; either version 2.1 of
 * the License, or (at your option) any later version.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this software; if not, write to the Free
 * Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301 USA, or see the FSF site: http://www.fsf.org.
 */

#include "mqtt.h"

MQTT::MQTT(QObject *parent) : QObject(parent)
{
    this->socket = new ClientSocket(parent);
    connect(this->socket, SIGNAL(connect()),                                this, SLOT(connectSlot()));
    connect(this->socket, SIGNAL(received(QByteArray)),                     this, SLOT(receivedSlot(QByteArray)));
    connect(this->socket, SIGNAL(disconnect()),                             this, SLOT(disconnectSlot()));
    connect(this->socket, SIGNAL(withError(QAbstractSocket::SocketError)),  this, SLOT(errorSlot(QAbstractSocket::SocketError)));

    this->messageParser = new MessagesParser(parent);
    connect(this->messageParser, SIGNAL(messagesParserError(QString *)), this, SLOT(messagesParserErrorSlot(QString *)));
}

void MQTT::startTCPConnectionWith(QString &host, quint16 port)
{
    this->socket->connectToHost(host, port);
}

void MQTT::stopTCPConnection()
{
    if (this->socket->isConnect() == true) {
        this->socket->disconnectFromHost();
    }
}

bool MQTT::isConnected()
{
    return this->socket->isConnect();
}

void MQTT::sendMessage(Message *message)
{
    if (this->socket->isConnect() == true) {
        QByteArray byteArray = this->messageParser->encodeMessage(message);
        if (this->socket->sendData(byteArray) == true) {
            emit messageDidSend(message);
        }
    }
}

void MQTT::connectSlot()
{
    emit TCPDidStart();
}

void MQTT::disconnectSlot()
{
    emit TCPDidDisconnect();
}

void MQTT::receivedSlot(QByteArray data)
{
    do {
        QByteArray barray = this->messageParser->nextMessage(data);
        Message *message = this->messageParser->decodeMessage(barray);
        if (message != NULL && message->getType() >= CONNECT && message->getType() <= DISCONNECT) {
            emit messageDidReceive(message);
        }
    } while (data.length() > 0);
}

void MQTT::errorSlot(QAbstractSocket::SocketError code)
{
    emit errorMessage(code);
}

void MQTT::messagesParserErrorSlot(QString *error)
{
    emit parserError(error);
}
