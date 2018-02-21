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
