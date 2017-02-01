#ifndef CONNACK_H
#define CONNACK_H

#include <parser/messages/message.h>

enum ConnackCode
{
    ACCEPTED = 0,
    UNACCEPTABLE_PROTOCOL_VERSION = 1,
    IDENTIFIER_REJECTED = 2,
    SERVER_UNUVALIABLE = 3,
    BAD_USER_OR_PASS = 4,
    NOT_AUTHORIZED = 5
};

/**
 * @brief The Connack class
 */

class Connack : public Message
{  
private:
    bool sessionPresent;
    ConnackCode returnCode;

public:
    Connack();
    Connack(bool sessionPresent, ConnackCode returnCode);

    virtual int getLength();
    virtual MessageType getType();

    bool isSessionPresent();
    void setSessionPresent(bool sessionPresent);
    ConnackCode getReturnCode();
    void setReturnCode(ConnackCode returnCode);

    bool isValidReturnCode(ConnackCode code);
};

#endif // CONNACK_H
