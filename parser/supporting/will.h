#ifndef WILL_H
#define WILL_H

#include <QByteArray>
#include <parser/supporting/topic.h>

/**
 * @brief The Will class
 */

class Will
{

private:
    Topic *topic;
    QByteArray content;
    bool retain;

public:
    Will();
    Will(Topic *topic, QByteArray content, bool retain);

    int retrieveLength();

    Topic *getTopic();
    void setTopic(Topic *topic);

    QByteArray getContent();
    void setContent(QByteArray content);

    bool isRetain();
    void setRetain(bool retain);

    bool isValid();
};

#endif // WILL_H
