#ifndef PARSER_H
#define PARSER_H

#include <QDebug>
#include <QBuffer>
#include <QByteArray>
#include <parser/supporting/bytearray.h>
#include <parser/supporting/lengthdetails.h>

#include <parser/messages/message.h>
#include <parser/messages/connect.h>
#include <parser/messages/connack.h>
#include <parser/messages/disconnect.h>
#include <parser/messages/pingreq.h>
#include <parser/messages/pingresp.h>
#include <parser/messages/publish.h>
#include <parser/messages/puback.h>
#include <parser/messages/pubcomp.h>
#include <parser/messages/pubrec.h>
#include <parser/messages/pubrel.h>
#include <parser/messages/subscribe.h>
#include <parser/messages/suback.h>
#include <parser/messages/unsubscribe.h>
#include <parser/messages/unsuback.h>

/**
 * @brief The Parser class
 */

class Parser
{

private:
    QByteArray getBufferByLength(int length);

public:

    QByteArray next(QByteArray &byteArray);

    QByteArray encode(Message *message);
    Message *decode(QByteArray byteArray);
};

#endif // PARSER_H
