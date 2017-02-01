#ifndef PUBLISH_H
#define PUBLISH_H

#include <parser/messages/countablemessage.h>
#include <parser/supporting/topic.h>
#include <QByteArray>

/**
 * @brief The Publish class
 */

class Publish : public CountableMessage
{
private:
    int packetID;
    Topic *topic;
    QByteArray content;
    bool retain;
    bool dup;

public:
    Publish();
    Publish(int packetID);
    Publish(Topic *topic, QByteArray content, bool retain, bool dup);
    Publish(int packetID, Topic *topic, QByteArray content, bool retain, bool dup);

    virtual int getPacketID();
    virtual void setPacketID(int packetID);

    virtual int getLength();
    virtual MessageType getType();

    Topic *getTopic();
    void setTopic(Topic *topic);

    QByteArray getContent();
    void setContent(QByteArray content);

    bool isRetain();
    void setRetain(bool retain);

    bool isDup();
    void setDup(bool dup);
};

#endif // PUBLISH_H
