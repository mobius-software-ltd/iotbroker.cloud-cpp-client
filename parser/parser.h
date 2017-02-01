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
