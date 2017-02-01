#ifndef TIMERSMAP_H
#define TIMERSMAP_H

#include <QObject>
#include <timer/timertask.h>
#include <mqtt/mqttmanager.h>

static int const MAX_VALUE = 65535;
static int const FIRST_ID = 1;
static int const MESSAGE_RESEND_PERIOD = 3000;

/**
 * @brief The TimersMap class
 */

class TimersMap
{
private:
    QMap<int, TimerTask *> *timersMap;

    TimerTask *connect;
    TimerTask *ping;

    int count;

    MQTTManager *mqtt;

public:
    TimersMap(MQTTManager *mqtt);

    void goConnectTimer(Connect *connect);
    void stopConnectTimer();

    void goPingTimer(int keepalive);
    void stopPingTimer();

    void goMessageTimer(Message *message);

    Message *removeTimer(int id);
    Message *stopTimer(int id);

    void stopAllTimers();

    int getNewPacketID();
};

#endif // TIMERSMAP_H
