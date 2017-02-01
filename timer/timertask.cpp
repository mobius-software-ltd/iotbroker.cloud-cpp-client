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
