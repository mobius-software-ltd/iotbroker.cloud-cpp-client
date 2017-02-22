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

#include "unsubscribe.h"

Unsubscribe::Unsubscribe()
{
    this->packetID = 0;
}

Unsubscribe::Unsubscribe(QList<QString> *topics)
{
    this->packetID = 0;
    this->topics = topics;
}

Unsubscribe::Unsubscribe(int packetID, QList<QString> *topics)
{
    this->packetID = packetID;
    this->topics = topics;
}

int Unsubscribe::getPacketID()
{
    return this->packetID;
}

void Unsubscribe::setPacketID(int packetID)
{
    this->packetID = packetID;
}

int Unsubscribe::getLength()
{
    int length = 2;
    for (int i = 0; i < this->topics->size(); i++) {
        QString item = this->topics->at(i);
        length += item.length() + 2;
    }
    return length;
}

MessageType Unsubscribe::getType()
{
    return UNSUBSCRIBE;
}

QList<QString> *Unsubscribe::getTopics()
{
    return topics;
}

void Unsubscribe::setTopics(QList<QString> *topics)
{
    this->topics = topics;
}