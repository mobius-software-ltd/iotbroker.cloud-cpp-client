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

#ifndef SUBSCRIBE_H
#define SUBSCRIBE_H

#include <parser/messages/countablemessage.h>
#include <parser/supporting/topic.h>
#include <QList>

/**
 * @brief The Subscribe class
 */

class Subscribe : public CountableMessage
{
private:
    int packetID;
    QList<Topic> *topics;

public:
    Subscribe();
    Subscribe(int packetID);
    Subscribe(QList<Topic> *topics);
    Subscribe(int packetID, QList<Topic> *topics);

    virtual int getPacketID();
    virtual void setPacketID(int packetID);

    virtual int getLength();
    virtual MessageType getType();

    QList<Topic> *getTopics();
    void setTopics(QList<Topic> *topics);
};

#endif // SUBSCRIBE_H
