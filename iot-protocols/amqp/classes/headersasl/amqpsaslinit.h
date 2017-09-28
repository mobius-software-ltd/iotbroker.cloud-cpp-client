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

#ifndef AMQPSASLINIT_H
#define AMQPSASLINIT_H

#include "iot-protocols/amqp/classes/headerapi/amqpheader.h"
#include "iot-protocols/amqp/classes/wrappers/amqpsymbol.h"

class AMQPSASLInit : public AMQPHeader
{
private:

    AMQPSymbol *mechanism;
    ByteArray initialResponse;
    QString hostName;

public:
    AMQPSASLInit();

    virtual int getLength();
    virtual int getType();

    virtual AMQPTLVList *arguments();
    virtual void fillArguments(AMQPTLVList *list);

    AMQPSymbol *getMechanism() const;
    void setMechanism(AMQPSymbol *value);

    ByteArray getInitialResponse() const;
    void setInitialResponse(const ByteArray &value);

    QString getHostName() const;
    void setHostName(const QString &value);
};

#endif // AMQPSASLINIT_H
