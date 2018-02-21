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

#ifndef AMQPTARGET_H
#define AMQPTARGET_H

#include "iot-protocols/amqp/classes/enums/amqpterminusdurability.h"
#include "iot-protocols/amqp/classes/enums/amqpterminusexpirypolicy.h"
#include "iot-protocols/amqp/classes/enums/amqpdistributionmode.h"
#include "iot-protocols/amqp/classes/wrappers/amqpsymbol.h"
#include "iot-protocols/amqp/classes/tlv/described/amqpoutcome.h"

class AMQPTarget
{
private:

    QString address;
    AMQPTerminusDurability *durable;
    AMQPTerminusExpiryPolicy *expiryPeriod;
    QVariant *timeout;
    QVariant *dynamic;
    QMap<QVariant *, QVariant *> dynamicNodeProperties;
    QList<QVariant *> capabilities;

public:
    AMQPTarget();

    AMQPTLVList *getlist();
    void fill(AMQPTLVList *list);

    void addDynamicNodeProperties(QString key, QVariant *value);
    void addCapabilities(QList<QString> array);

    QString getAddress() const;
    void setAddress(const QString &value);
    AMQPTerminusDurability *getDurable() const;
    void setDurable(AMQPTerminusDurability *value);
    AMQPTerminusExpiryPolicy *getExpiryPeriod() const;
    void setExpiryPeriod(AMQPTerminusExpiryPolicy *value);
    QVariant *getTimeout() const;
    void setTimeout(QVariant *value);
    QVariant *getDynamic() const;
    void setDynamic(QVariant *value);
    QMap<QVariant *, QVariant *> getDynamicNodeProperties() const;
    void setDynamicNodeProperties(const QMap<QVariant *, QVariant *> &value);
    QList<QVariant *> getCapabilities() const;
    void setCapabilities(const QList<QVariant *> &value);
};

#endif // AMQPTARGET_H
