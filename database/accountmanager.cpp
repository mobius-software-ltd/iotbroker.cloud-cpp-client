/**
 * Mobius Software LTD
 * Copyright 2015-2017, Mobius Software LTD
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

#include "accountmanager.h"

AccountManager* AccountManager::instance = 0;

AccountManager::AccountManager()
{
    this->dataBase = DataBase();
    qDebug() << qApp->applicationDirPath() + QDir::separator() + "iotbroker_database.db";
    this->dataBase.createDataBaseWith(qApp->applicationDirPath() + QDir::separator() + "iotbroker_database.db");
}

AccountManager::~AccountManager()
{

}

AccountManager* AccountManager::getInstance()
{
    if (!instance) {
        instance = new AccountManager();
    }
    return instance;
}

bool AccountManager::isDefaultAccountExist()
{
    bool flag = false;

    if (this->dataBase.open() == true) {
        DQQuery<AccountEntity> query;
        query = query.filter(DQWhere("isDefault") == true);
        if (query.exec()) {
            while (query.next()) {
                flag = true;
            }
        }
    }
    this->dataBase.close();
    return flag;
}

void AccountManager::addAccount(AccountEntity account)
{
    if (this->dataBase.open() == true) {
        account.save();
    }
    this->dataBase.close();
}

DQList<AccountEntity> AccountManager::accounts()
{
    DQList<AccountEntity> list;
    if (this->dataBase.open() == true) {
        DQQuery<AccountEntity> query;
        list = query.all();
    }
    this->dataBase.close();
    return list;
}

void AccountManager::setDefaultAccountWithClientID(QString clientID)
{
    AccountEntity account = this->getAccountWithClientID(clientID);

    if (account.id.get().isValid()) {

        this->uncheckDefaultAccount();

        if (this->dataBase.open() == true) {
            account.isDefault = true;
            account.save();
        }
    }
    this->dataBase.close();
}

void AccountManager::uncheckDefaultAccount()
{
    AccountEntity account = this->readDefaultAccount();

    if (account.id.get().isValid()) {

        if (this->dataBase.open() == true) {

            account.isDefault = false;
            account.save();
        }
    }
    this->dataBase.close();
}

AccountEntity AccountManager::getAccountWithClientID(QString clientID)
{
    AccountEntity account = AccountEntity();

    if (this->dataBase.open() == true) {
        DQQuery<AccountEntity> query;
        query = query.filter(DQWhere("clientID") == clientID);
        if (query.exec()) {
            while (query.next()) {
                query.recordTo(account);
            }
        }
    }
    this->dataBase.close();
    return account;
}

AccountEntity AccountManager::readDefaultAccount()
{
    AccountEntity account = AccountEntity();

    if (this->dataBase.open() == true) {
        DQQuery<AccountEntity> query;
        query = query.filter(DQWhere("isDefault") == true);
        if (query.exec()) {
            while (query.next()) {
                query.recordTo(account);
            }
        }
    }
    this->dataBase.close();
    return account;
}

void AccountManager::addTopicForDefaultAccount(TopicEntity topic)
{
    AccountEntity account = this->readDefaultAccount();

    if (this->dataBase.open() == true) {
        topic.account = account;
        topic.save(true);
    }
    this->dataBase.close();
}

QList<TopicEntity> AccountManager::topicsForAccount(AccountEntity account)
{
    QList<TopicEntity> topics = QList<TopicEntity>();

    if (this->dataBase.open() == true) {
        DQQuery<TopicEntity> query;
        query = query.filter(DQWhere("account") == account.id);
        if (query.exec()) {
            while (query.next()) {
                TopicEntity topic;
                query.recordTo(topic);
                topics.append(topic);
            }
        }
    }
    this->dataBase.close();
    return topics;
}

QList<TopicEntity> AccountManager::topicsForDefaultAccount()
{
    AccountEntity account = this->readDefaultAccount();
    return this->topicsForAccount(account);
}

void AccountManager::addMessageForDefaultAccount(MessageEntity message, bool incoming)
{
    AccountEntity account = this->readDefaultAccount();

    if (this->dataBase.open() == true) {
        message.incoming = incoming;
        message.account = account;
        message.save(true);
    }
    this->dataBase.close();
}

QList<MessageEntity> AccountManager::messagesForAccount(AccountEntity account)
{
    QList<MessageEntity> messages = QList<MessageEntity>();

    if (this->dataBase.open() == true) {
        DQQuery<MessageEntity> query;
        query = query.filter(DQWhere("account") == account.id);
        if (query.exec()) {
            while (query.next()) {
                MessageEntity message;
                query.recordTo(message);
                messages.append(message);
            }
        }
    }
    this->dataBase.close();
    return messages;
}

QList<MessageEntity> AccountManager::messagesForDefaultAccount()
{
    AccountEntity account = this->readDefaultAccount();
    return this->messagesForAccount(account);
}

void AccountManager::deleteAccount(AccountEntity account)
{
    QList<MessageEntity> messages = this->messagesForAccount(account);

    QList<TopicEntity> topics = this->topicsForAccount(account);

    for (int i = 0; i < messages.size(); i++) {
        this->deleteMessage(messages.at(i));
    }

    for (int i = 0; i < topics.size(); i++) {
        this->deleteTopic(topics.at(i));
    }

    if (this->dataBase.open() == true) {
        account.remove();
    }
    this->dataBase.close();
}

void AccountManager::deleteTopic(TopicEntity topic)
{
    if (this->dataBase.open() == true) {
        topic.remove();
    }
    this->dataBase.close();
}

void AccountManager::deleteMessage(MessageEntity message)
{
    if (this->dataBase.open() == true) {
        message.remove();
    }
    this->dataBase.close();
}

TopicEntity AccountManager::topicByName(QString name)
{
    TopicEntity topic = TopicEntity();

    if (this->dataBase.open() == true) {
        DQQuery<TopicEntity> query;
        query = query.filter(DQWhere("topicName") == name);
        if (query.exec()) {
            while (query.next()) {
                query.recordTo(topic);
            }
        }
    }
    this->dataBase.close();
    return topic;
}