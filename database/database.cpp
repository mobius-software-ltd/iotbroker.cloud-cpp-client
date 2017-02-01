#include "database.h"

DataBase::DataBase()
{

}

void DataBase::createDataBaseWith(QString path)
{
    this->path = path;
    this->dataBase = QSqlDatabase::addDatabase("QSQLITE");
    this->dataBase.setDatabaseName(this->path);
}

bool DataBase::addEntity(AccountEntity entity, bool forceInsert)
{
    if (this->open() == true) {
        entity.save(forceInsert);
        this->close();
        return true;
    }
    return false;
}

bool DataBase::addEntity(MessageEntity entity, bool forceInsert)
{
    if (this->open() == true) {
        entity.save(forceInsert);
        this->close();
        return true;
    }
    return false;
}

bool DataBase::addEntity(TopicEntity entity, bool forceInsert)
{
    if (this->open() == true) {
        entity.save(forceInsert);
        this->close();
        return true;
    }
    return false;
}

bool DataBase::open()
{
    if (this->dataBase.open() == true) {
        this->connection = connection;
        if (this->connection.open(this->dataBase) == true) {
            this->connection.addModel<AccountEntity>();
            this->connection.addModel<MessageEntity>();
            this->connection.addModel<TopicEntity>();
            this->connection.createTables();
            return true;
        }
        return false;
    }
    return false;
}

void DataBase::close()
{
    this->connection.close();
    this->dataBase.close();
}
