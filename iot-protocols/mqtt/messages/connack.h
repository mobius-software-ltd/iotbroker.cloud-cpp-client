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

#ifndef CONNACK_H
#define CONNACK_H

#include <parser/messages/message.h>

enum ConnackCode
{
    ACCEPTED = 0,
    UNACCEPTABLE_PROTOCOL_VERSION = 1,
    IDENTIFIER_REJECTED = 2,
    SERVER_UNUVALIABLE = 3,
    BAD_USER_OR_PASS = 4,
    NOT_AUTHORIZED = 5
};

/**
 * @brief The Connack class
 */

class Connack : public Message
{  
private:
    bool sessionPresent;
    ConnackCode returnCode;

public:
    Connack();
    Connack(bool sessionPresent, ConnackCode returnCode);

    virtual int getLength();
    virtual MessageType getType();

    bool isSessionPresent();
    void setSessionPresent(bool sessionPresent);
    ConnackCode getReturnCode();
    void setReturnCode(ConnackCode returnCode);

    bool isValidReturnCode(ConnackCode code);
};

#endif // CONNACK_H
