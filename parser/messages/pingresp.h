#ifndef PINGRESP_H
#define PINGRESP_H

#include <parser/messages/message.h>

/**
 * @brief The Pingresp class
 */

class Pingresp : public Message
{
public:
    Pingresp();

    virtual int getLength();
    virtual MessageType getType();
};

#endif // PINGRESP_H
