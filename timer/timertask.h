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

#ifndef TIMERTASK_H
#define TIMERTASK_H

#include <QObject>
#include <QTimer>
#include <QThread>

#include "iot-protocols/classes/message.h"

/**
 * @brief The TimerTask class
 */

class IotProtocol;

class TimerTask : public QObject
{
    Q_OBJECT

private:
    Message *message;
    IotProtocol *iotProtocol;
    int period;
    bool status;

    QTimer *timer;
    QThread *thread;

    int connectCount;

public:
    TimerTask(Message *message, IotProtocol *iotProtocol, int period);

    int getPeriod();
    Message *getMessage();

    void start();
    void start(bool withDelay);

    void stop();

public slots:
    void startSlot();
};

#endif // TIMERTASK_H
