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

#ifndef MESSAGESPARSER_H
#define MESSAGESPARSER_H

#include <QObject>
#include "iot-protocols/mqtt/parser/parser.h"

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
