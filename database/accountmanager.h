/**
 * Mobius Software LTD
 * Copyright 2015-2018, Mobius Software LTD
 *
 * This is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation; either version 2.1 of
 * the License, or (at your option) any later version.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this software; if not, write to the Free
 * Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301 USA, or see the FSF site: http://www.fsf.org.
 */

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

    bool isAccountValid(AccountEntity account);

    bool isDefaultAccountExist();

    bool isAccountWithClientIdExist(QString clientId);

    void addAccount(AccountEntity account);
    DQList<AccountEntity> accounts();
    void setDefaultAccountWithClientID(QString clientID);
    void uncheckDefaultAccount();
    AccountEntity getAccountWithClientID(QString clientID);
    AccountEntity readDefaultAccount();

    void addTopicForAccount(AccountEntity account,TopicEntity topic);
    QList<TopicEntity> topicsForAccount(AccountEntity account);

    void addMessageForAccount(AccountEntity account,MessageEntity message, bool incoming);
    QList<MessageEntity> messagesForAccount(AccountEntity account);

    void deleteAccount(AccountEntity account);
    void deleteTopic(TopicEntity topic);
    void deleteMessage(MessageEntity message);

    bool isTopicExist(AccountEntity account,QString topic);

    void removeTopicsForAccount(AccountEntity account);
    void removeMessagesForAccount(AccountEntity account);

    TopicEntity topicByName(QString name);
};

#endif // ACCOUNTMANAGER_H
