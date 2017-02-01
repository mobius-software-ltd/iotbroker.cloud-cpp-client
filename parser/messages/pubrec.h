#ifndef PUBREC_H
#define PUBREC_H

#include <parser/messages/countablemessage.h>

/**
 * @brief The Pubrec class
 */

class Pubrec : public CountableMessage
{
private:
    int packetID;

public:
    Pubrec();
    Pubrec(int packetID);

    virtual int getPacketID();
    virtual void setPacketID(int packetID);

    virtual int getLength();
    virtual MessageType getType();
};

#endif // PUBREC_H
