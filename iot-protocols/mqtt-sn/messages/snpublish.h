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

#ifndef SNPUBLISH_H
#define SNPUBLISH_H

#include "iot-protocols/classes/topic.h"
#include "iot-protocols/classes/message.h"
#include "iot-protocols/mqtt-sn/classes/mqttsnenums.h"

class SNPublish : public Message
{

private:

    int packetID;
    Topic *topic;
    QByteArray *content;
    bool isDup;
    bool isRetain;

public:
    SNPublish();
    SNPublish(int packetID, Topic *topic, QByteArray *content, bool isDup, bool isRetain);

    virtual int getLength();
    virtual int getType();
    virtual IotEnumProtocol *getProtocol();

    int getPacketID();
    void setPacketID(int value);

    Topic *getTopic() const;
    void setTopic(Topic *value);

    QByteArray *getContent() const;
    void setContent(QByteArray *value);

    bool getIsDup() const;
    void setIsDup(bool value);

    bool getIsRetain() const;
    void setIsRetain(bool value);
};

#endif // SNPUBLISH_H
