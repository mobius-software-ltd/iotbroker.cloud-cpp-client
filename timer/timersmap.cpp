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

#include "timersmap.h"

TimersMap::TimersMap(MQTTManager *mqtt)
{
    this->timersMap = new QMap<int, TimerTask *>();
    this->connect = NULL;
    this->ping = NULL;
    this->mqtt = mqtt;
    this->count = 0;
}

void TimersMap::goConnectTimer(Connect *connect)
{
    if (this->connect != NULL) {
        this->connect->stop();
    }

    this->connect = new TimerTask(connect, this->mqtt->getMQTT(), MESSAGE_RESEND_PERIOD);
    this->connect->start();
}

void TimersMap::stopConnectTimer()
{
    if (this->connect != NULL) {
        this->connect->stop();
        this->connect = NULL;
    }
}

void TimersMap::goPingTimer(int keepalive)
{
    if (this->ping != NULL) {
        this->ping->stop();
    }
    this->ping = new TimerTask(new Pingreq(), this->mqtt->getMQTT(), keepalive * 1000);
    this->ping->start();
}

void TimersMap::stopPingTimer()
{
    if (this->ping != NULL) {
        this->ping->stop();
        this->ping = NULL;
    }
}

void TimersMap::goMessageTimer(Message *message)
{
    TimerTask *timer = new TimerTask(message, this->mqtt->getMQTT(), MESSAGE_RESEND_PERIOD);

    if (this->timersMap->size() == MAX_VALUE) {
        throw new QString("TimersMap : Outgoing identifier overflow");
    }

    CountableMessage *countableMessage  = (CountableMessage *)message;
    if (countableMessage->getPacketID() == 0) {
        int packetID = this->getNewPacketID();
        countableMessage->setPacketID(packetID);
    }

    this->timersMap->insert(countableMessage->getPacketID(), timer);

    timer->start();
}

Message *TimersMap::removeTimer(int id)
{
    TimerTask *timer = this->timersMap->value(id);
    Message *message = timer->getMessage();

    if (timer != NULL) {
        timer->stop();
    }

    this->timersMap->remove(id);

    return message;
}

Message *TimersMap::stopTimer(int id)
{
    TimerTask *timer = this->timersMap->value(id);

    if (timer != NULL) {
        Message *message = timer->getMessage();
        timer->stop();
        return message;
    }

    return NULL;
}

void TimersMap::stopAllTimers()
{
    this->stopConnectTimer();
    this->stopPingTimer();

    QList<int> keys = this->timersMap->keys();
    for (int i = 0; i < this->timersMap->size(); i++) {
        TimerTask *timer = this->timersMap->value(keys.at(i));
        if (timer != NULL) {
            timer->stop();
        }
        this->timersMap->remove(keys.at(i));
    }

    this->count = 0;
}

int TimersMap::getNewPacketID()
{
    this->count++;

    if (this->count == MAX_VALUE) {
        this->count = FIRST_ID;
    }

    return this->count;
}