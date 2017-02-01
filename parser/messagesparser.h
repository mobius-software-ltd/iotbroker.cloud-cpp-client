#ifndef MESSAGESPARSER_H
#define MESSAGESPARSER_H

#include <QObject>
#include <parser/parser.h>

/**
 * @brief The MessagesParser class
 */

class MessagesParser : public QObject
{
    Q_OBJECT

private:
    Parser parser;

public:
    explicit MessagesParser(QObject *parent = 0);

    Message *decodeMessage(QByteArray data);
    QByteArray encodeMessage(Message *message);

    QByteArray nextMessage(QByteArray &byteArray);

signals:
    void messagesParserError(QString *error);
};

#endif // MESSAGESPARSER_H
