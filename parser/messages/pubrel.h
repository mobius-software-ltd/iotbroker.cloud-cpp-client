#ifndef PUBREL_H
#define PUBREL_H

#include <parser/messages/countablemessage.h>

/**
 * @brief The Pubrel class
 */

class Pubrel : public CountableMessage
{
private:
    int packetID;

public:
    Pubrel();
    Pubrel(int packetID);

    virtual int getPacketID();
    virtual void setPacketID(int packetID);

    virtual int getLength();
    virtual MessageType getType();
};

#endif // PUBREL_H
