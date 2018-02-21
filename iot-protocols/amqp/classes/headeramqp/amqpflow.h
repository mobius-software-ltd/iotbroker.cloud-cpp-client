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

#ifndef AMQPFLOW_H
#define AMQPFLOW_H

#include "iot-protocols/amqp/classes/headerapi/amqpheader.h"

class AMQPFlow : public AMQPHeader
{
private:

    QVariant *nextIncomingId;
    QVariant *incomingWindow;
    QVariant *nextOutgoingId;
    QVariant *outgoingWindow;
    QVariant *handle;
    QVariant *deliveryCount;
    QVariant *linkCredit;
    QVariant *avaliable;
    QVariant *drain;
    QVariant *echo;
    QMap<QVariant *, QVariant *> properties;

public:
    AMQPFlow();

    virtual int getLength();
    virtual int getType();

    virtual AMQPTLVList *arguments();
    virtual void fillArguments(AMQPTLVList *list);

    void addProperty(QString key, QVariant *value);

    QVariant *getNextIncomingId() const;
    void setNextIncomingId(QVariant *value);

    QVariant *getIncomingWindow() const;
    void setIncomingWindow(QVariant *value);

    QVariant *getNextOutgoingId() const;
    void setNextOutgoingId(QVariant *value);

    QVariant *getOutgoingWindow() const;
    void setOutgoingWindow(QVariant *value);

    QVariant *getHandle() const;
    void setHandle(QVariant *value);

    QVariant *getDeliveryCount() const;
    void setDeliveryCount(QVariant *value);

    QVariant *getLinkCredit() const;
    void setLinkCredit(QVariant *value);

    QVariant *getAvaliable() const;
    void setAvaliable(QVariant *value);

    QVariant *getDrain() const;
    void setDrain(QVariant *value);

    QVariant *getEcho() const;
    void setEcho(QVariant *value);

    QMap<QVariant *, QVariant *> getProperties() const;

    void setProperties(const QMap<QVariant *, QVariant *> &value);
};

#endif // AMQPFLOW_H
