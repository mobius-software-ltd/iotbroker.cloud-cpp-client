#ifndef PINGREQ_H
#define PINGREQ_H

#include <parser/messages/message.h>

/**
 * @brief The Pingreq class
 */

class Pingreq : public Message
{
public:
    Pingreq();

    virtual int getLength();
    virtual MessageType getType();
};

#endif // PINGREQ_H
