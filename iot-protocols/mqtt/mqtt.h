/**
 * Mobius Software LTD
 * Copyright 2015-2018, Mobius Software LTD
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

#include "iot-protocols/iotprotocol.h"
#include "iot-protocols/mqtt/parser/messagesparser.h"

class MQTT : public IotProtocol
{

protected:

    int keepAlive;
    Connect *connect;
    QMap<int, Message *> *publishPackets;
    MessagesParser *messageParser;

public:
    MQTT(AccountEntity account);

    virtual bool send(Message *message);

    virtual void goConnect();
    virtual void publish(MessageEntity message);
    virtual void subscribeTo(TopicEntity topic);
    virtual void unsubscribeFrom(TopicEntity topic);
    virtual void pingreq();
    virtual void disconnectWith(int duration);

    virtual Message *getPingreqMessage();

    virtual void timeoutMethod();

public slots:

    void connectionDidStart(InternetProtocol *protocol);
    void connectionDidStop(InternetProtocol *protocol);
    void didReceiveMessage(InternetProtocol *protocol, QByteArray data);
    void didFailWithError(InternetProtocol *protocol, QString error);

    void parseFailWithError(QString *error);
};

#endif // MQTT_H
