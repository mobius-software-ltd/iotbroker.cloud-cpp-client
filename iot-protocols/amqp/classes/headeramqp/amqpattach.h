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

#ifndef AMQPATTACH_H
#define AMQPATTACH_H

#include <QMap>
#include <QString>
#include <QVariant>
#include "iot-protocols/amqp/classes/headerapi/amqpheader.h"
#include "iot-protocols/amqp/classes/enums/amqprolecode.h"
#include "iot-protocols/amqp/classes/enums/amqpsendcode.h"
#include "iot-protocols/amqp/classes/enums/amqpreceiversettlemode.h"
#include "iot-protocols/amqp/classes/terminus/amqpsource.h"
#include "iot-protocols/amqp/classes/terminus/amqptarget.h"
#include "iot-protocols/amqp/classes/wrappers/amqpsymbol.h"

class AMQPAttach : public AMQPHeader
{
private:

    QString name;
    QVariant *handle;
    AMQPRoleCode *role;
    AMQPSendCode *sendCodes;
    AMQPReceiverSettleMode *receivedCodes;
    AMQPSource *source;
    AMQPTarget *target;
    QMap<QVariant *, QVariant *> unsettled;
    QVariant *incompleteUnsettled;
    QVariant *initialDeliveryCount;
    QVariant *maxMessageSize;
    QList<QVariant *> offeredCapabilities;
    QList<QVariant *> desiredCapabilities;
    QMap<QVariant *, QVariant *> properties;

public:
    AMQPAttach();

    virtual int getLength();
    virtual int getType();

    virtual AMQPTLVList *arguments();
    virtual void fillArguments(AMQPTLVList *list);

    void addUnsettled(QString key, QVariant *value);
    void addOfferedCapability(QList<QString> array);
    void addDesiredCapability(QList<QString> array);
    void addProperty(QString key, QVariant *value);

    QString getName() const;
    void setName(const QString &value);

    QVariant *getHandle() const;
    void setHandle(QVariant *value);

    AMQPRoleCode *getRole() const;
    void setRole(AMQPRoleCode *value);

    AMQPSendCode *getSendCodes() const;
    void setSendCodes(AMQPSendCode *value);

    AMQPReceiverSettleMode *getReceivedCodes() const;
    void setReceivedCodes(AMQPReceiverSettleMode *value);

    AMQPSource *getSource() const;
    void setSource(AMQPSource *value);

    AMQPTarget *getTarget() const;
    void setTarget(AMQPTarget *value);

    QMap<QVariant *, QVariant *> getUnsettled() const;
    void setUnsettled(const QMap<QVariant *, QVariant *> &value);

    QVariant *getIncompleteUnsettled() const;
    void setIncompleteUnsettled(QVariant *value);

    QVariant *getInitialDeliveryCount() const;
    void setInitialDeliveryCount(QVariant *value);

    QVariant *getMaxMessageSize() const;
    void setMaxMessageSize(QVariant *value);

    QList<QVariant *> getOfferedCapabilities() const;
    void setOfferedCapabilities(const QList<QVariant *> &value);

    QList<QVariant *> getDesiredCapabilities() const;
    void setDesiredCapabilities(const QList<QVariant *> &value);

    QMap<QVariant *, QVariant *> getProperties() const;
    void setProperties(const QMap<QVariant *, QVariant *> &value);
};

#endif // AMQPATTACH_H
