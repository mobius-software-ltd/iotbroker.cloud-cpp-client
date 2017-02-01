#ifndef DATABASE_H
#define DATABASE_H

#include <database/entities/messageentity.h>
#include <database/entities/topicentity.h>

/**
 * @brief The DataBase class
 */

class DataBase
{
private:
    QString path;
    QSqlDatabase dataBase;
    DQConnection connection;

public:
    DataBase();

    void createDataBaseWith(QString path);
    bool open();
    void close();

    bool addEntity(AccountEntity entity, bool forceInsert = false);
    bool addEntity(MessageEntity entity, bool forceInsert = false);
    bool addEntity(TopicEntity entity, bool forceInsert = false);
};

#endif // DATABASE_H

