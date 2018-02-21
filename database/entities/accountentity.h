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

#ifndef ACCOUNTENTITY_H
#define ACCOUNTENTITY_H

#include <dquest.h>

/**
 * @brief The AccountEntity class
 */

class AccountEntity : public DQModel
{
    DQ_MODEL
public:

    DQField<int>        protocol;
    DQField<QString>    username;
    DQField<QString>    password;
    DQField<QString>    clientID;
    DQField<QString>    serverHost;
    DQField<int>        port;
    DQField<bool>       cleanSession;
    DQField<int>        keepAlive;
    DQField<QString>    will;
    DQField<QString>    willTopic;
    DQField<bool>       isRetain;
    DQField<int>        qos;
    DQField<bool>       isDefault;
};

DQ_DECLARE_MODEL(AccountEntity,
                 "account",
                 DQ_FIELD(protocol),
                 DQ_FIELD(username , DQNotNull),
                 DQ_FIELD(password),
                 DQ_FIELD(clientID),
                 DQ_FIELD(serverHost),
                 DQ_FIELD(port),
                 DQ_FIELD(cleanSession),
                 DQ_FIELD(keepAlive),
                 DQ_FIELD(will),
                 DQ_FIELD(willTopic),
                 DQ_FIELD(isRetain),
                 DQ_FIELD(qos),
                 DQ_FIELD(isDefault)
                 )

#endif // ACCOUNT_H
