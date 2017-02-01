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

#include "clientsocket.h"

ClientSocket::ClientSocket(QObject *parent) : QObject(parent)
{
    this->socket = new QTcpSocket(parent);

    QObject::connect(socket, SIGNAL(aboutToClose()),                                             this, SLOT(aboutToCloseSlot()));
    QObject::connect(socket, SIGNAL(bytesWritten(qint64)),                                       this, SLOT(bytesWrittenSlot(qint64)));
    QObject::connect(socket, SIGNAL(channelBytesWritten(int,qint64)),                            this, SLOT(channelBytesWrittenSlot(int,qint64)));
    QObject::connect(socket, SIGNAL(channelReadyRead(int)),                                      this, SLOT(channelReadyReadSlot(int)));
    QObject::connect(socket, SIGNAL(destroyed(QObject*)),                                        this, SLOT(destroyedSlot(QObject*)));
    QObject::connect(socket, SIGNAL(disconnected()),                                             this, SLOT(disconnectedSlot()));
    QObject::connect(socket, SIGNAL(error(QAbstractSocket::SocketError)),                        this, SLOT(errorSlot(QAbstractSocket::SocketError)));
    QObject::connect(socket, SIGNAL(hostFound()),                                                this, SLOT(hostDidFoundSlot()));
    QObject::connect(socket, SIGNAL(objectNameChanged(QString)),                                 this, SLOT(objectNameChangedSlot(QString)));
    QObject::connect(socket, SIGNAL(readChannelFinished()),                                      this, SLOT(readChannelFinishedSlot()));
    QObject::connect(socket, SIGNAL(readyRead()),                                                this, SLOT(readyReadSlot()));
    QObject::connect(socket, SIGNAL(stateChanged(QAbstractSocket::SocketState)),                 this, SLOT(stateDidChangedSlot(QAbstractSocket::SocketState)));
}

void ClientSocket::connectToHost(QString &host, quint16 port)
{
    if (isConnect() == true) {
        this->socket->disconnectFromHost();
    }
    this->socket->connectToHost(host, port);
}

bool ClientSocket::sendData(QByteArray data)
{
    if (isConnect() == true) {
        qint64 bytes = this->socket->write(data.data(), data.size());
        if (bytes == -1) {
            return false;
        }
    }
    return false;
}

void ClientSocket::disconnectFromHost()
{
    if (isConnect() == true) {
        this->socket->disconnectFromHost();
    }
}

bool ClientSocket::isConnect()
{
    return this->socket->state() == QAbstractSocket::ConnectedState;
}

/**
  *
  * SLOTS
  *
  */

void ClientSocket::aboutToCloseSlot()
{
    //qDebug() << " >> SOCKET. aboutToCloseSlot()";
}

void ClientSocket::bytesWrittenSlot(qint64 bytes)
{
    //qDebug() << " >> SOCKET. bytesWrittenSlot(" << bytes << ")";
}

void ClientSocket::channelBytesWrittenSlot(int channel, qint64 bytes)
{
    //qDebug() << " >> SOCKET. channelBytesWrittenSlot(" << channel << ", " << bytes << ")";
}

void ClientSocket::channelReadyReadSlot(int channel)
{
    //qDebug() << " >> SOCKET. channelReadyReadSlot("<< channel <<")";

    QByteArray byteArray = QByteArray();

    while (socket->bytesAvailable()) {
       //qDebug() << " !> bytesAvailable = " << socket->bytesAvailable();
       byteArray.append(socket->readAll());

    }

    if (byteArray.size() > 0) {
        emit received(byteArray);
    }
}

void ClientSocket::destroyedSlot(QObject *object)
{
    //qDebug() << " >> SOCKET. destroyedSlot("<< object <<")";
}

void ClientSocket::disconnectedSlot()
{
    //qDebug() << " >> SOCKET. disconnectedSlot()";
    emit disconnect();
}

void ClientSocket::errorSlot(QAbstractSocket::SocketError error)
{
    //qDebug() << " >> SOCKET. errorSlot("<< error <<")";
    emit withError(error);
}

void ClientSocket::hostDidFoundSlot()
{
    //qDebug() << " >> SOCKET. hostDidFoundSlot()";
}

void ClientSocket::objectNameChangedSlot(QString objectName)
{
    //qDebug() << " >> SOCKET. objectNameChangedSlot("<< objectName <<")";
}

void ClientSocket::readChannelFinishedSlot()
{
    //qDebug() << " >> SOCKET. readChannelFinishedSlot()";
}

void ClientSocket::readyReadSlot()
{
    //qDebug() << " >> SOCKET. readyReadSlot()";
}

void ClientSocket::stateDidChangedSlot(QAbstractSocket::SocketState state)
{
    //qDebug() << " >> SOCKET. stateDidChangedSlot("<< state <<")";

    if (state == QAbstractSocket::ConnectedState) {
        emit connect();
    }
}
