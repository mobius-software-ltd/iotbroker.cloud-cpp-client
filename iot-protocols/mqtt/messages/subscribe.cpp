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

#include "subscribe.h"

Subscribe::Subscribe()
{
    this->packetID = 0;
}

Subscribe::Subscribe(int packetID)
{
    this->packetID = packetID;
}

Subscribe::Subscribe(QList<Topic> *topics)
{
    this->packetID = 0;
    this->topics = topics;
}

Subscribe::Subscribe(int packetID, QList<Topic> *topics)
{
    this->packetID = packetID;
    this->topics = topics;
}

int Subscribe::getPacketID()
{
    return this->packetID;
}

void Subscribe::setPacketID(int packetID)
{
    this->packetID = packetID;
}

int Subscribe::getLength()
{
    int length = 0;
    length += this->packetID != 0 ? 2 : 0;
    for (int i = 0; i < this->topics->size(); i++) {
        Topic item = this->topics->at(i);
        length += item.getName().length() + 3;
    }
    return length;
}

MessageType Subscribe::getType()
{
    return SUBSCRIBE;
}

QList<Topic> *Subscribe::getTopics()
{
    return this->topics;
}

void Subscribe::setTopics(QList<Topic> *topics)
{
    this->topics = topics;
}
