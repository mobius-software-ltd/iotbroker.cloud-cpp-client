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

#include "snvaluesvalidator.h"
#include <QDebug>

static QList<int> *reservedPacketIDS = NULL;
static QList<int> *reservedTopicIDS = NULL;

SNValuesValidator::SNValuesValidator()
{

}

bool SNValuesValidator::validatePacketID(int packetID)
{
    reservedPacketIDS = new QList<int>();
    reservedPacketIDS->append(0x0000);
    return packetID > 0 && !reservedPacketIDS->contains(packetID);
}

bool SNValuesValidator::validateTopicID(int topicID)
{
    reservedTopicIDS = new QList<int>();
    reservedTopicIDS->append(0x0000);
    reservedTopicIDS->append(0xFFFF);
    return topicID > 0 && !reservedTopicIDS->contains(topicID);
}

bool SNValuesValidator::validateRegistrationTopicID(int topicID)
{
    return topicID >= 0;
}

bool SNValuesValidator::canReadData(ByteArray data, int left)
{
    return (data.getSize() > 0 && left > 0);
}

bool SNValuesValidator::validateClientID(QString *clientID)
{
    return (clientID != NULL && clientID->length() != 0);
}
