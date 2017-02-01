#ifndef TOPIC_H
#define TOPIC_H

#include <parser/supporting/qos.h>
#include <QString>

/**
 * @brief The Topic class
 */

class Topic
{

private:
    QString name;
    QoS *qos;

public:
    Topic();
    Topic(QString name, QoS *qos);

    QString toString();

    QString getName() const;
    void setName(QString name);

    QoS *getQoS() const;
    void setQoS(QoS *qos);

    int length();
};

#endif // TOPIC_H
