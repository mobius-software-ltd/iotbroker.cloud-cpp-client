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

#ifndef IOTPROTOCOL_H
#define IOTPROTOCOL_H

#include <QObject>
#include "iot-protocols/classes/message.h"
#include "internet-protocols/internetprotocol.h"
#include "database/entities/accountentity.h"
#include "database/entities/messageentity.h"
#include "database/entities/topicentity.h"
#include "timer/timersmap.h"

class IotProtocol : public QObject
{
    Q_OBJECT

protected:
    InternetProtocol *internetProtocol;
    AccountEntity currentAccount;
    TimersMap *timers;
    bool isConnect;

public:
    IotProtocol();
    IotProtocol(AccountEntity account);

    InternetProtocol *getInternetProtocol() const;
    void setInternetProtocol(InternetProtocol *value);

    bool getIsConnect() const;

    virtual bool send(Message *message) = 0;

    virtual void goConnect() = 0;
    virtual void publish(MessageEntity message) = 0;
    virtual void subscribeTo(TopicEntity topic) = 0;
    virtual void unsubscribeFrom(TopicEntity topic) = 0;
    virtual void pingreq() = 0;
    virtual void disconnectWith(int duration) = 0;

    virtual Message *getPingreqMessage() = 0;

    virtual void timeoutMethod() = 0;

public slots:

    virtual void connectionDidStart(InternetProtocol *protocol) = 0;
    virtual void connectionDidStop(InternetProtocol *protocol) = 0;
    virtual void didReceiveMessage(InternetProtocol *protocol, QByteArray data) = 0;
    virtual void didFailWithError(InternetProtocol *protocol, QString error) = 0;

signals:

    void ready(IotProtocol *protocol);
    void connackReceived(IotProtocol *protocol, int returnCode);
    void publishReceived(IotProtocol *protocol, QString topic, int qos, QByteArray content, bool dup, bool retainFlag);
    void pubackReceived(IotProtocol *protocol, QString topic, int qos, QByteArray content, bool dup, bool retainFlag, int returnCode);
    void subackReceived(IotProtocol*protocol, QString topic, int qos, int returnCode);
    void unsubackReceived(IotProtocol*protocol, QString topic);
    void pingrespReceived(IotProtocol*protocol);
    void disconnectReceived(IotProtocol*protocol);
    void timeout(IotProtocol*protocol);
    void errorReceived(IotProtocol*protocol, QString error);
};

#endif // IOTPROTOCOL_H
