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

#ifndef CLIENTSOCKET_H
#define CLIENTSOCKET_H

#include <QObject>
#include <QTcpSocket>
#include <QByteArray>
#include <QDebug>
#include <QNetworkProxy>
#include <QAuthenticator>

/**
 * @brief The ClientSocket class
 */

class ClientSocket : public QObject
{
    Q_OBJECT

private:
    QTcpSocket *socket;

public:
    explicit ClientSocket(QObject *parent = 0);

    void connectToHost(QString &host, quint16 port);
    bool sendData(QByteArray data);
    void disconnectFromHost();

    bool isConnect();

public slots:

    void aboutToCloseSlot();
    void bytesWrittenSlot(qint64 bytes);
    void channelBytesWrittenSlot(int channel, qint64 bytes);
    void channelReadyReadSlot(int channel);
    void destroyedSlot(QObject *object);
    void disconnectedSlot();
    void errorSlot(QAbstractSocket::SocketError error);
    void hostDidFoundSlot();
    void objectNameChangedSlot(QString objectName);
    void readChannelFinishedSlot();
    void readyReadSlot();
    void stateDidChangedSlot(QAbstractSocket::SocketState state);

signals:

    void connect();
    void withError(QAbstractSocket::SocketError error);
    void received(QByteArray data);
    void disconnect();
};

#endif // CLIENTSOCKET_H
