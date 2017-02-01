#ifndef ACCOUNTENTITY_H
#define ACCOUNTENTITY_H

#include <dquest.h>

/**
 * @brief The AccountEntity class
 */

class AccountEntity : public DQModel
{
    DQ_MODEL
public:

    DQField<QString>    username;
    DQField<QString>    password;
    DQField<QString>    clientID;
    DQField<QString>    serverHost;
    DQField<int>        port;
    DQField<bool>       cleanSession;
    DQField<int>        keepAlive;
    DQField<QString>    will;
    DQField<QString>    willTopic;
    DQField<bool>       isRetain;
    DQField<int>        qos;
    DQField<bool>       isDefault;
};

DQ_DECLARE_MODEL(AccountEntity,
                 "account",
                 DQ_FIELD(username , DQNotNull),
                 DQ_FIELD(password),
                 DQ_FIELD(clientID),
                 DQ_FIELD(serverHost),
                 DQ_FIELD(port),
                 DQ_FIELD(cleanSession),
                 DQ_FIELD(keepAlive),
                 DQ_FIELD(will),
                 DQ_FIELD(willTopic),
                 DQ_FIELD(isRetain),
                 DQ_FIELD(qos),
                 DQ_FIELD(isDefault)
                 )

#endif // ACCOUNT_H
