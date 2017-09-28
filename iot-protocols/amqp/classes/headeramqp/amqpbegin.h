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

#ifndef AMQPBEGIN_H
#define AMQPBEGIN_H

#include "iot-protocols/amqp/classes/headerapi/amqpheader.h"
#include "iot-protocols/amqp/classes/wrappers/amqpsymbol.h"

class AMQPBegin : public AMQPHeader
{
private:

    QVariant *remoteChannel;
    QVariant *nextOutgoingID;
    QVariant *incomingWindow;
    QVariant *outgoingWindow;
    QVariant *handleMax;
    QList<QVariant *> offeredCapabilities;
    QList<QVariant *> desiredCapabilities;
    QMap<QVariant *, QVariant *> properties;

public:
    AMQPBegin();

    virtual int getLength();
    virtual int getType();

    virtual AMQPTLVList *arguments();
    virtual void fillArguments(AMQPTLVList *list);

    void addOfferedCapability(QList<QString> list);
    void addDesiredCapability(QList<QString> list);
    void addProperty(QString key, QVariant *value);

    QVariant *getRemoteChannel() const;
    void setRemoteChannel(QVariant *value);

    QVariant *getNextOutgoingID() const;
    void setNextOutgoingID(QVariant *value);

    QVariant *getIncomingWindow() const;
    void setIncomingWindow(QVariant *value);

    QVariant *getOutgoingWindow() const;
    void setOutgoingWindow(QVariant *value);

    QVariant *getHandleMax() const;
    void setHandleMax(QVariant *value);

    QList<QVariant *> getOfferedCapabilities() const;
    void setOfferedCapabilities(const QList<QVariant *> &value);

    QList<QVariant *> getDesiredCapabilities() const;
    void setDesiredCapabilities(const QList<QVariant *> &value);

    QMap<QVariant *, QVariant *> getProperties() const;
    void setProperties(const QMap<QVariant *, QVariant *> &value);
};

#endif // AMQPBEGIN_H
