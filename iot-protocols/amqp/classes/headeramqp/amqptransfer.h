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

#ifndef AMQPTRANSFER_H
#define AMQPTRANSFER_H

#include "iot-protocols/amqp/classes/headerapi/amqpheader.h"
#include "iot-protocols/amqp/classes/tlv/described/amqpstate.h"
#include "iot-protocols/amqp/classes/wrappers/amqpmessageformat.h"
#include "iot-protocols/amqp/classes/enums/amqpreceiversettlemode.h"
#include "iot-protocols/amqp/classes/enums/amqpsectioncode.h"
#include "iot-protocols/amqp/classes/section/amqpsection.h"

class AMQPTransfer : public AMQPHeader
{
    Q_OBJECT
private:

    QVariant *handle;
    QVariant *deliveryId;
    ByteArray deliveryTag;
    AMQPMessageFormat *messageFormat;
    QVariant *settled;
    QVariant *more;
    AMQPReceiverSettleMode *rcvSettleMode;
    AMQPState *state;
    QVariant *resume;
    QVariant *aborted;
    QVariant *batchable;
    QMap<AMQPSectionCode *, AMQPSection *> sections;

public:
    AMQPTransfer();

    virtual int getLength();
    virtual int getType();

    virtual AMQPTLVList *arguments();
    virtual void fillArguments(AMQPTLVList *list);

    AMQPSection *getHeader();
    AMQPSection *getDeliveryAnnotations();
    AMQPSection *getMessageAnnotations();
    AMQPSection *getProperties();
    AMQPSection *getApplicationProperties();
    AMQPSection *getData();
    AMQPSection *getSequence();
    AMQPSection *getValue();
    AMQPSection *getFooter();

    void addSections(QList<AMQPSection *> array);

    QVariant *getHandle() const;
    void setHandle(QVariant *value);

    QVariant *getDeliveryId() const;
    void setDeliveryId(QVariant *value);

    ByteArray getDeliveryTag() const;
    void setDeliveryTag(const ByteArray &value);

    AMQPMessageFormat *getMessageFormat() const;
    void setMessageFormat(AMQPMessageFormat *value);

    QVariant *getSettled() const;
    void setSettled(QVariant *value);

    QVariant *getMore() const;
    void setMore(QVariant *value);

    AMQPReceiverSettleMode *getRcvSettleMode() const;
    void setRcvSettleMode(AMQPReceiverSettleMode *value);

    AMQPState *getState() const;
    void setState(AMQPState *value);

    QVariant *getResume() const;
    void setResume(QVariant *value);

    QVariant *getAborted() const;
    void setAborted(QVariant *value);

    QVariant *getBatchable() const;
    void setBatchable(QVariant *value);

    QMap<AMQPSectionCode *, AMQPSection *> getSections() const;
    void setSections(const QMap<AMQPSectionCode *, AMQPSection *> &value);
};

#endif // AMQPTRANSFER_H
