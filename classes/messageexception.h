#ifndef MESSAGEEXCEPTION_H
#define MESSAGEEXCEPTION_H

#include <QString>

class MessageException
{
private:
    QString message;

public:
    MessageException(QString message);

    void raise() const { throw *this; }
    MessageException *clone() const { return new MessageException(*this); }

    QString getMessage() const;
};

#endif // MESSAGEEXCEPTION_H
