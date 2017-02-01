#ifndef COUNTABLEMESSAGE_H
#define COUNTABLEMESSAGE_H

#include <parser/messages/message.h>

class CountableMessage : public Message
{
public:

    virtual int getPacketID() = 0;
    virtual void setPacketID(int packetID) = 0;
};

#endif // COUNTABLEMESSAGE_H
