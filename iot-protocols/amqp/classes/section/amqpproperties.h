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

#ifndef AMQPPROPERTIES_H
#define AMQPPROPERTIES_H

#include "iot-protocols/amqp/classes/section/amqpsection.h"
#include "iot-protocols/amqp/classes/wrappers/messageID/amqpmessageid.h"
#include <QDateTime>

class AMQPProperties : public AMQPSection
{
private:

    AMQPMessageID *messageId;
    ByteArray userId;
    QString to;
    QString subject;
    QString replyTo;
    ByteArray correlationID;
    QString contentType;
    QString contentEncoding;
    QDateTime absoluteExpiryTime;
    QDateTime creationTime;
    QString groupId;
    QVariant *groupSequence;
    QString replyToGroupId;

public:
    AMQPProperties();

    virtual TLVAMQP *getValue();
    virtual AMQPSectionCode *getCode();

    virtual void fill(TLVAMQP *value);

    AMQPMessageID *getMessageId() const;
    void setMessageId(AMQPMessageID *value);

    ByteArray getUserId() const;
    void setUserId(const ByteArray &value);

    QString getTo() const;
    void setTo(const QString &value);

    QString getSubject() const;
    void setSubject(const QString &value);

    QString getReplyTo() const;
    void setReplyTo(const QString &value);

    ByteArray getCorrelationID() const;
    void setCorrelationID(const ByteArray &value);

    QString getContentType() const;
    void setContentType(const QString &value);

    QString getContentEncoding() const;
    void setContentEncoding(const QString &value);

    QDateTime getAbsoluteExpiryTime() const;
    void setAbsoluteExpiryTime(const QDateTime &value);

    QDateTime getCreationTime() const;
    void setCreationTime(const QDateTime &value);

    QString getGroupId() const;
    void setGroupId(const QString &value);

    QVariant *getGroupSequence() const;
    void setGroupSequence(QVariant *value);

    QString getReplyToGroupId() const;
    void setReplyToGroupId(const QString &value);
};

#endif // AMQPPROPERTIES_H
