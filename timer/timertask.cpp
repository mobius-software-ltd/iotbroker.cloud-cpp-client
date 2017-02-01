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

#include "timertask.h"

TimerTask::TimerTask(Message *message, MQTT *mqtt, int period)
{
    this->timer = new QTimer();
    this->message = message;
    this->mqtt = mqtt;
    this->period = period;
    this->status = false;
}

int TimerTask::getPeriod()
{
    return this->period;
}

Message *TimerTask::getMessage()
{
    return this->message;
}

void TimerTask::start()
{
    this->timer->setTimerType(Qt::PreciseTimer);
    QObject::connect(this->timer, SIGNAL(timeout()), this, SLOT(startSlot()));
    this->timer->start(this->period);
}

void TimerTask::startSlot()
{
    if (this->mqtt->isConnected() == true) {
        if (this->status == true) {
            if (this->message->getType() == PUBLISH) {
                Publish *publish = (Publish *)message;
                publish->setDup(true);
            }
        }
        this->mqtt->sendMessage(this->message);
        this->status = true;
    }
}

void TimerTask::stop()
{
    this->timer->stop();
    this->status = false;
}
