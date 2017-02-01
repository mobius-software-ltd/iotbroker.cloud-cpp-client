#ifndef ACCOUNTMANAGER_H
#define ACCOUNTMANAGER_H

#include <database/database.h>

/**
 * @brief The AccountManager class
 */

class AccountManager
{
private:
    DataBase dataBase;

private:
    AccountManager();
    ~AccountManager();
    static AccountManager *instance;

public:
    static AccountManager *getInstance();

    bool isDefaultAccountExist();

    void addAccount(AccountEntity account);
    DQList<AccountEntity> accounts();
    void setDefaultAccountWithClientID(QString clientID);
    void uncheckDefaultAccount();
    AccountEntity getAccountWithClientID(QString clientID);
    AccountEntity readDefaultAccount();

    void addTopicForDefaultAccount(TopicEntity topic);
    QList<TopicEntity> topicsForAccount(AccountEntity account);
    QList<TopicEntity> topicsForDefaultAccount();

    void addMessageForDefaultAccount(MessageEntity message, bool incoming);
    QList<MessageEntity> messagesForAccount(AccountEntity account);
    QList<MessageEntity> messagesForDefaultAccount();

    void deleteAccount(AccountEntity account);
    void deleteTopic(TopicEntity topic);
    void deleteMessage(MessageEntity message);

    TopicEntity topicByName(QString name);
};

#endif // ACCOUNTMANAGER_H
