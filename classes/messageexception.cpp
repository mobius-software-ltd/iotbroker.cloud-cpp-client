#include "messageexception.h"

MessageException::MessageException(QString message)
{
    this->message = message;
}

QString MessageException::getMessage() const
{
    return message;
}
