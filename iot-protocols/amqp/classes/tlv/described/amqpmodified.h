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

#ifndef AMQPMODIFIED_H
#define AMQPMODIFIED_H

#include "iot-protocols/amqp/classes/tlv/described/amqpoutcome.h"
#include "iot-protocols/amqp/classes/tlv/described/amqpstate.h"
#include "iot-protocols/amqp/classes/wrappers/amqpsymbol.h"
#include <QMap>

class AMQPModified : public AMQPOutcome, public AMQPState
{
private:

    QVariant *deliveryFailed;
    QVariant *undeliverableHere;
    QMap<QVariant *, QVariant *> messageAnnotations;


public:
    AMQPModified();

    virtual void fill(AMQPTLVList *list);
    virtual AMQPTLVList *getList();

    void addMessageAnnotation(QString key, QVariant *value);

    QVariant *getDeliveryFailed() const;
    void setDeliveryFailed(QVariant *value);

    QVariant *getUndeliverableHere() const;
    void setUndeliverableHere(QVariant *value);

    QMap<QVariant *, QVariant *> getMessageAnnotations() const;
    void setMessageAnnotations(const QMap<QVariant *, QVariant *> &value);
};

#endif // AMQPMODIFIED_H
