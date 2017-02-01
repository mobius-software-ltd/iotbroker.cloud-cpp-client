#ifndef CONNECT_H
#define CONNECT_H

#include <parser/messages/message.h>
#include <QString>
#include <parser/supporting/will.h>

/**
 * @brief The Connect class
 */

class Connect : public Message
{
private:
    QString username;
    QString password;
    QString clientID;

    unsigned char protocolLevel;
    bool cleanSession;
    int keepAlive;

    Will *will;

public:
    Connect();
    Connect(QString username, QString password, QString clientID, bool cleanSession, int keepAlive, Will *will);

    virtual int getLength();
    virtual MessageType getType();

    QString getProtocolName();

    int getProtocolLevel();
    void setProtocolLevel(int protocolLevel);

    bool isCleanSession();
    void setCleanSession(bool cleanSession);

    bool isWillFlag();
    Will *getWill();
    void setWill(Will *will);

    int getKeepAlive();
    void setKeepAlive(bool keepAlive);

    QString getClientID();
    void setClientID(QString clientID);

    bool isUsernameFlag();
    QString getUsername();
    void setUsername(QString username);

    bool isPasswordFlag();
    QString getPassword();
    void setPassword(QString password);
};

#endif // CONNECT_H
