#ifndef SUBACK_H
#define SUBACK_H

#include <parser/messages/countablemessage.h>
#include <QList>

enum SubackCode
{
    ACCEPTED_QOS0 = 0,
    ACCEPTED_QOS1 = 1,
    ACCEPTED_QOS2 = 2,
    FAILURE = 128
};

/**
 * @brief The Suback class
 */

class Suback : public CountableMessage
{
private:
    int packetID;
    QList<SubackCode> *returnCodes;

public:
    Suback();
    Suback(int packetID);
    Suback(int packetID, QList<SubackCode> *returnCodes);

    virtual int getPacketID();
    virtual void setPacketID(int packetID);

    virtual int getLength();
    virtual MessageType getType();

    QList<SubackCode> *getReturnCodes();
    void setReturnCodes(QList<SubackCode> *returnCodes);

    bool isValidCode(SubackCode code);
};

#endif // SUBACK_H
