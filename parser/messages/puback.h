#ifndef PUBACK_H
#define PUBACK_H

#include <parser/messages/countablemessage.h>

/**
 * @brief The Puback class
 */

class Puback : public CountableMessage
{
private:
    int packetID;

public:
    Puback();
    Puback(int packetID);

    virtual int getPacketID();
    virtual void setPacketID(int packetID);

    virtual int getLength();
    virtual MessageType getType();
};

#endif // PUBACK_H
