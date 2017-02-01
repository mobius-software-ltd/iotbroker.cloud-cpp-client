#ifndef TOPICENTITY_H
#define TOPICENTITY_H

#include <dquest.h>
#include <database/entities/accountentity.h>

/**
 * @brief The TopicEntity class
 */

class TopicEntity : public DQModel
{
    DQ_MODEL
public:

    DQField<QString>            topicName;
    DQField<int>                qos;
    DQForeignKey<AccountEntity> account;
};

DQ_DECLARE_MODEL(TopicEntity,
                 "topic",
                 DQ_FIELD(topicName , DQNotNull),
                 DQ_FIELD(qos),
                 DQ_FIELD(account)
                 )

#endif // TOPICENTITY_H
