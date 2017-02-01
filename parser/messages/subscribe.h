#ifndef SUBSCRIBE_H
#define SUBSCRIBE_H

#include <parser/messages/countablemessage.h>
#include <parser/supporting/topic.h>
#include <QList>

/**
 * @brief The Subscribe class
 */

class Subscribe : public CountableMessage
{
private:
    int packetID;
    QList<Topic> *topics;

public:
    Subscribe();
    Subscribe(int packetID);
    Subscribe(QList<Topic> *topics);
    Subscribe(int packetID, QList<Topic> *topics);

    virtual int getPacketID();
    virtual void setPacketID(int packetID);

    virtual int getLength();
    virtual MessageType getType();

    QList<Topic> *getTopics();
    void setTopics(QList<Topic> *topics);
};

#endif // SUBSCRIBE_H
