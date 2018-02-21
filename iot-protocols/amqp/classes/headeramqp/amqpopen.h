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

#ifndef AMQPOPEN_H
#define AMQPOPEN_H

#include "iot-protocols/amqp/classes/headerapi/amqpheader.h"

class AMQPOpen : public AMQPHeader
{
private:

    QString containerId;
    QString hostname;
    QVariant *maxFrameSize;
    QVariant *channelMax;
    QVariant *idleTimeout;
    QList<QVariant *> outgoingLocales;
    QList<QVariant *> incomingLocales;
    QList<QVariant *> offeredCapabilities;
    QList<QVariant *> desiredCapabilities;
    QMap<QVariant *, QVariant *> properties;

public:
    AMQPOpen();

    virtual int getLength();
    virtual int getType();

    virtual AMQPTLVList *arguments();
    virtual void fillArguments(AMQPTLVList *list);

    void addOutgoingLocale(QList<QString> array);
    void addIncomingLocale(QList<QString> array);
    void addOfferedCapability(QList<QString> array);
    void addDesiredCapability(QList<QString> array);
    void addProperty(QString key, QVariant *value);

    QString getContainerId() const;
    void setContainerId(const QString &value);

    QString getHostname() const;
    void setHostname(const QString &value);

    QVariant *getMaxFrameSize() const;
    void setMaxFrameSize(QVariant *value);

    QVariant *getChannelMax() const;
    void setChannelMax(QVariant *value);

    QVariant *getIdleTimeout() const;
    void setIdleTimeout(QVariant *value);

    QList<QVariant *> getOutgoingLocales() const;
    void setOutgoingLocales(const QList<QVariant *> &value);

    QList<QVariant *> getIncomingLocales() const;
    void setIncomingLocales(const QList<QVariant *> &value);

    QList<QVariant *> getOfferedCapabilities() const;
    void setOfferedCapabilities(const QList<QVariant *> &value);

    QList<QVariant *> getDesiredCapabilities() const;
    void setDesiredCapabilities(const QList<QVariant *> &value);

    QMap<QVariant *, QVariant *> getProperties() const;
    void setProperties(const QMap<QVariant *, QVariant *> &value);
};

#endif // AMQPOPEN_H
