#ifndef TIMERTASK_H
#define TIMERTASK_H

#include <QObject>
#include <parser/messages/message.h>
#include <mqtt/mqtt.h>

/**
 * @brief The TimerTask class
 */

class TimerTask : public QObject
{
    Q_OBJECT

private:
    Message *message;
    MQTT *mqtt;
    int period;
    bool status;

    QTimer *timer;

public:
    explicit TimerTask(QObject *parent = 0);
    TimerTask(Message *message, MQTT *mqtt, int period);

    int getPeriod();
    Message *getMessage();

    void start();
    void stop();

public slots:
    void startSlot();
};

#endif // TIMERTASK_H
