#ifndef UNSUBACK_H
#define UNSUBACK_H

#include <parser/messages/countablemessage.h>

/**
 * @brief The Unsuback class
 */

class Unsuback : public CountableMessage
{
private:
    int packetID;

public:
    Unsuback();
    Unsuback(int packetID);

    virtual int getPacketID();
    virtual void setPacketID(int packetID);

    virtual int getLength();
    virtual MessageType getType();
};

#endif // UNSUBACK_H
