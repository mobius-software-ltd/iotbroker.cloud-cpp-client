#ifndef MESSAGEENTITY_H
#define MESSAGEENTITY_H

#include <dquest.h>
#include <database/entities/accountentity.h>

/**
 * @brief The MessageEntity class
 */

class MessageEntity : public DQModel
{
    DQ_MODEL
public:

    DQField<QByteArray>             content;
    DQField<int>                    qos;
    DQField<QString>                topicName;
    DQField<bool>                   incoming;
    DQForeignKey<AccountEntity>     account;
};

DQ_DECLARE_MODEL(MessageEntity,
                 "message",
                 DQ_FIELD(content , DQNotNull),
                 DQ_FIELD(qos),
                 DQ_FIELD(topicName),
                 DQ_FIELD(incoming),
                 DQ_FIELD(account)
                 )

#endif // MESSAGEENTITY_H
