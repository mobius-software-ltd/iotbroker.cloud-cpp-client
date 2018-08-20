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

#include "timertask.h"
#include "iot-protocols/iotprotocol.h"
#include "iot-protocols/mqtt/messages/publish.h"
#include "iot-protocols/mqtt/classes/mqttenums.h"
#include "classes/scheduler.h"

TimerTask::TimerTask(Message *message, IotProtocol *iotProtocol, int period)
{
    this->thread = new QThread(this);
    this->timer = new QTimer(0);
    this->timer->setTimerType(Qt::PreciseTimer);
    this->timer->setInterval(period);
    this->timer->moveToThread(this->thread);

    QObject::connect(this->timer, SIGNAL(timeout()), this, SLOT(startSlot()));
    QObject::connect(this->thread, SIGNAL(started()), this->timer, SLOT(start()));
    QObject::connect(this->thread, SIGNAL(finished()), this->timer, SLOT(stop()));

    this->message = message;
    this->iotProtocol = iotProtocol;
    this->period = period;
    this->status = false;

    this->isTimeoutTask = false;
}

int TimerTask::getPeriod()
{
    return this->period;
}

Message *TimerTask::getMessage()
{
    return this->message;
}

void TimerTask::setIsTimeoutTask(bool value)
{
    this->isTimeoutTask = value;
}

void TimerTask::start()
{
    this->thread->start();
    this->thread->setPriority(QThread::TimeCriticalPriority);
}

void TimerTask::startSlot()
{
    if (this->isTimeoutTask == true) {
        this->iotProtocol->timeoutMethod();
        return;
    }

    if (this->iotProtocol->getIsConnect() == true) {
        if (this->status == true) {
            if (this->message->getType() == MQ_PUBLISH) {
                Publish *publish = (Publish *)message;
                publish->setDup(true);
            }
        }
        this->iotProtocol->send(this->message);
        this->status = true;
    }
}

void TimerTask::stop()
{
    if (!this->thread->isFinished()) {
        this->thread->terminate();
    }
    this->status = false;
}
