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

#ifndef SNMESSAGESPARSER_H
#define SNMESSAGESPARSER_H

#include <QObject>
#include "iot-protocols/mqtt-sn/parser/snparser.h"

class SNMessagesParser : public QObject
{
    Q_OBJECT

private:
    SNParser parser;

public:
    explicit SNMessagesParser(QObject *parent = 0);

    Message *decodeMessage(QByteArray data);
    QByteArray encodeMessage(Message *message);

signals:
    void messagesParserError(QString *error);

};

#endif // SNMESSAGESPARSER_H
