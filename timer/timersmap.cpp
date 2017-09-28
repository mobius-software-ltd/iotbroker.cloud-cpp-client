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
#include "iot-protocols/classes/countablemessage.h"
#include "iot-protocols/iotprotocol.h"

TimersMap::TimersMap(IotProtocol *iotProtocol)
{
    this->timersMap = new QMap<int, TimerTask *>();
    this->connect = NULL;
    this->ping = NULL;
    this->reg = NULL;
    this->timeout = NULL;
    this->iotProtocol = iotProtocol;
    this->count = 0;
}

void TimersMap::goConnectTimer(Message *connect)
{
    if (this->connect != NULL) {
        this->connect->stop();
    }

    this->connect = new TimerTask(connect, this->iotProtocol, 3000);
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

    this->ping = new TimerTask(this->iotProtocol->getPingreqMessage(), this->iotProtocol, keepalive * 1000);
    this->ping->start();
}

void TimersMap::stopPingTimer()
{
    if (this->ping != NULL) {
        this->ping->stop();
        this->ping = NULL;
    }
}

void TimersMap::goRegisterTimer(Message *reg)
{
    if (this->reg != NULL) {
        this->reg->stop();
    }

    SNRegister *regPacket  = (SNRegister *)reg;
    if (regPacket->getPacketID() == 0) {
        int packetID = this->getNewPacketID();
        regPacket->setPacketID(packetID);
    }

    this->reg = new TimerTask(reg, this->iotProtocol, 3000);
    this->reg->start();
}

void TimersMap::stopRegisterTimer()
{
    if (this->reg != NULL) {
        this->reg->stop();
        this->reg = NULL;
    }
}

void TimersMap::goTimeoutTimer()
{
    if (this->timeout != NULL) {
        this->timeout->stop();
    }

    this->timeout = new TimerTask(NULL, this->iotProtocol, TIMEOUT_VALUE);
    this->timeout->setIsTimeoutTask(true);
    this->timeout->start();
}

void TimersMap::stopTimeoutTimer()
{
    if (this->timeout != NULL) {
        this->timeout->stop();
        this->timeout = NULL;
    }
}

void TimersMap::goCoAPMessageTimer(Message *message)
{
    TimerTask *timer = new TimerTask(message, this->iotProtocol, MESSAGE_RESEND_PERIOD);

    CoAPMessage *coapMessage = (CoAPMessage *)message;
    this->timersMap->insert(coapMessage->getToken(), timer);

    timer->start();
}

void TimersMap::goMessageTimer(Message *message)
{
    TimerTask *timer = new TimerTask(message, this->iotProtocol, MESSAGE_RESEND_PERIOD);

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
    this->stopRegisterTimer();
    this->stopTimeoutTimer();

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
