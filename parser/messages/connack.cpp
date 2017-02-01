#include "connack.h"

Connack::Connack()
{

}

Connack::Connack(bool sessionPresent, ConnackCode returnCode)
{
    this->sessionPresent = sessionPresent;
    this->returnCode = returnCode;
}

int Connack::getLength()
{
    return 2;
}

MessageType Connack::getType()
{
    return CONNACK;
}

bool Connack::isSessionPresent()
{
    return this->sessionPresent;
}

void Connack::setSessionPresent(bool sessionPresent)
{
    this->sessionPresent = sessionPresent;
}

ConnackCode Connack::getReturnCode()
{
    return this->returnCode;
}

void Connack::setReturnCode(ConnackCode returnCode)
{
    this->returnCode = returnCode;
}

bool Connack::isValidReturnCode(ConnackCode code)
{
    if (code >= ACCEPTED && code <= NOT_AUTHORIZED) {
        return true;
    }
    return false;
}
