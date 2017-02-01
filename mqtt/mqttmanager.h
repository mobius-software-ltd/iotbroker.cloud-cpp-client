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

#ifndef MQTTMANAGER_H
#define MQTTMANAGER_H

#include <QObject>
#include <mqtt/mqtt.h>
#include <database/accountmanager.h>

/**
 * @brief The MQTTManager class
 */

class MQTTManager : public QObject
{
    Q_OBJECT

    MQTT *mqtt;
    AccountManager *accountManager;

public:
    explicit MQTTManager(QObject *parent = 0);

    void openConnectionWith(QString &host, quint16 port);

    bool connectMessage(Connect *connect);
    bool publishMessage(Publish *publish);
    bool pubackMessage(Puback *puback);
    bool pubrecMessage(Pubrec *pubrec);
    bool pubrelMessage(Pubrel *pubrel);
    bool pubcompMessage(Pubcomp *pubcomp);
    bool subscribeMessage(Subscribe *subscribe);
    bool unsubscribeMessage(Unsubscribe *unsubscribe);
    bool pingreqMessage();
    bool disconnectMessage();

    MQTT *getMQTT();

signals:

    void openConnection();
    void closeConnection();

    void connackDidReceive(Connack *connack);
    void publishDidReceive(Publish *publish);
    void pubackDidReceive(Puback *puback);
    void pubrecDidReceive(Pubrec *pubrec);
    void pubrelDidReceive(Pubrel *pubrel);
    void pubcompDidReceive(Pubcomp *pubcomp);
    void subackDidReceive(Suback *suback);
    void unsubackDidReceive(Unsuback *unsuback);
    void pingrespDidReceive(Pingresp *pingresp);

    void errorMessageDidReceive(QString *error);

    void errorString(QString *error);

public slots:

    void TCPDidStartSlot();
    void messageDidSendSlot(Message *message);
    void messageDidReceiveSlot(Message *message);
    void errorMessageSlot(QAbstractSocket::SocketError code);
    void parserErrorSlot(QString *error);
    void TCPDidDisconnectSlot();

};

#endif // MQTTMANAGER_H
