#ifndef MQJSONPARSER_H
#define MQJSONPARSER_H

#include <QByteArray>
#include "iot-protocols/classes/message.h"

class MQJsonParser
{

public:    

    static QByteArray json(Message *message);
    static Message *message(QByteArray json);

};

#endif // MQJSONPARSER_H
