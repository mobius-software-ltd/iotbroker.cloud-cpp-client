#include "messagesparser.h"
#include <QDebug>

MessagesParser::MessagesParser(QObject *parent) : QObject(parent)
{
    this->parser = Parser();
}

Message *MessagesParser::decodeMessage(QByteArray data)
{
    Message *message = NULL;
    try {
        message = this->parser.decode(data);
    } catch (QString *exception) {
        emit messagesParserError(exception);
    }
    return message;
}

QByteArray MessagesParser::encodeMessage(Message *message)
{
    QByteArray data = QByteArray();
    try {
        data = this->parser.encode(message);
    } catch (QString *exception) {
        emit messagesParserError(exception);
    }
    return data;
}

QByteArray MessagesParser::nextMessage(QByteArray &byteArray)
{
    QByteArray data = QByteArray();
    try {
        data = this->parser.next(byteArray);
    } catch (QString *exception) {
        emit messagesParserError(exception);
    }
    return data;
}
