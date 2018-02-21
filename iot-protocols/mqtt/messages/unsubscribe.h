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

#ifndef UNSUBSCRIBE_H
#define UNSUBSCRIBE_H

#include "iot-protocols/classes/countablemessage.h"
#include "iot-protocols/mqtt/classes/mqtopic.h"
#include <QList>
#include <QString>

/**
 * @brief The Unsubscribe class
 */

class Unsubscribe : public CountableMessage
{
private:
    int packetID;
    QList<QString> *topics;

public:
    Unsubscribe();
    Unsubscribe(QList<QString> *topics);
    Unsubscribe(int packetID, QList<QString> *topics);

    virtual int getPacketID();
    virtual void setPacketID(int packetID);

    virtual int getLength();
    virtual int getType();
    virtual IotEnumProtocol *getProtocol();

    QList<QString> *getTopics();
    void setTopics(QList<QString> *topics);
};

#endif // UNSUBSCRIBE_H
