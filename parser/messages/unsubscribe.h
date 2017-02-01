#ifndef UNSUBSCRIBE_H
#define UNSUBSCRIBE_H

#include <parser/messages/countablemessage.h>
#include <parser/supporting/topic.h>
#include <QList>
#include <QString>

/**
 * @brief The Unsubscribe class
 */

class Unsubscribe : public CountableMessage
{
private:
    int packetID;
    QList<QString> *topics;

public:
    Unsubscribe();
    Unsubscribe(QList<QString> *topics);
    Unsubscribe(int packetID, QList<QString> *topics);

    virtual int getPacketID();
    virtual void setPacketID(int packetID);

    virtual int getLength();
    virtual MessageType getType();

    QList<QString> *getTopics();
    void setTopics(QList<QString> *topics);
};

#endif // UNSUBSCRIBE_H
