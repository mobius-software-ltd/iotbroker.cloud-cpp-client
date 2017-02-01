#ifndef DISCONNECT_H
#define DISCONNECT_H

#include <parser/messages/message.h>

/**
 * @brief The Disconnect class
 */

class Disconnect : public Message
{
public:
    Disconnect();

    virtual int getLength();
    virtual MessageType getType();
};

#endif // DISCONNECT_H
