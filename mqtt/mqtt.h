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

#ifndef MQTT_H
#define MQTT_H

#include <QObject>
#include <parser/messagesparser.h>
#include <mqtt/socket/clientsocket.h>
#include <database/accountmanager.h>

/**
 * @brief The MQTT class
 */

class MQTT : public QObject
{
    Q_OBJECT

private:
    ClientSocket *socket;
    MessagesParser *messageParser;

public:
    explicit MQTT(QObject *parent = 0);

    void startTCPConnectionWith(QString &host, quint16 port);
    void stopTCPConnection();

    bool isConnected();

    void sendMessage(Message *message);

    QList<Message *> *decodeMessages(QByteArray data);
    QByteArray encodeMessages(QList<Message *> * messages);

signals:

    void TCPDidStart();
    void messageDidSend(Message *message);
    void messageDidReceive(Message *message);
    void errorMessage(QAbstractSocket::SocketError code);
    void parserError(QString *error);
    void TCPDidDisconnect();

public slots:

    void connectSlot();
    void disconnectSlot();
    void receivedSlot(QByteArray data);
    void errorSlot(QAbstractSocket::SocketError code);
    void messagesParserErrorSlot(QString *error);
};

#endif // MQTT_H
