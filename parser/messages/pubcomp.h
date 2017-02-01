#ifndef PUBCOMP_H
#define PUBCOMP_H

#include <parser/messages/countablemessage.h>

/**
 * @brief The Pubcomp class
 */

class Pubcomp : public CountableMessage
{
private:
    int packetID;

public:
    Pubcomp();
    Pubcomp(int packetID);

    virtual int getPacketID();
    virtual void setPacketID(int packetID);

    virtual int getLength();
    virtual MessageType getType();
};

#endif // PUBCOMP_H
