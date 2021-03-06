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

#ifndef AMQPDISPOSITION_H
#define AMQPDISPOSITION_H

#include "iot-protocols/amqp/classes/headerapi/amqpheader.h"
#include "iot-protocols/amqp/classes/tlv/described/amqpstate.h"
#include "iot-protocols/amqp/classes/wrappers/amqpsymbol.h"
#include "iot-protocols/amqp/classes/enums/amqprolecode.h"

class AMQPDisposition : public AMQPHeader
{
private:

    AMQPRoleCode *role;
    QVariant *first;
    QVariant *last;
    QVariant *settled;
    AMQPState *state;
    QVariant *batchable;

public:
    AMQPDisposition();

    virtual int getLength();
    virtual int getType();

    virtual AMQPTLVList *arguments();
    virtual void fillArguments(AMQPTLVList *list);

    AMQPRoleCode *getRole() const;
    void setRole(AMQPRoleCode *value);

    QVariant *getFirst() const;
    void setFirst(QVariant *value);

    QVariant *getLast() const;
    void setLast(QVariant *value);

    QVariant *getSettled() const;
    void setSettled(QVariant *value);

    AMQPState *getState() const;
    void setState(AMQPState *value);

    QVariant *getBatchable() const;
    void setBatchable(QVariant *value);
};

#endif // AMQPDISPOSITION_H
