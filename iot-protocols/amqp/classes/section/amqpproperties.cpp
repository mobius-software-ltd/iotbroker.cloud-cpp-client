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

#include "amqpproperties.h"
#include "iot-protocols/amqp/classes/headerapi/amqpwrapper.h"
#include "iot-protocols/amqp/classes/headerapi/amqpunwrapper.h"
#include "iot-protocols/amqp/classes/constructors/amqpdescribedconstructor.h"
#include "iot-protocols/amqp/classes/wrappers/messageID/amqpbinaryid.h"
#include "iot-protocols/amqp/classes/wrappers/messageID/amqplongid.h"
#include "iot-protocols/amqp/classes/wrappers/messageID/amqpmessageid.h"
#include "iot-protocols/amqp/classes/wrappers/messageID/amqpstringid.h"
#include "iot-protocols/amqp/classes/wrappers/messageID/amqpuuid.h"

AMQPProperties::AMQPProperties()
{

}

TLVAMQP *AMQPProperties::getValue()
{
    AMQPTLVList *list = new AMQPTLVList();

    if (this->messageId != NULL) {
        QVariant *object = NULL;
        if (!this->messageId->getBinary().getByteArray().isNull()) {
            object = AMQPSimpleType::binaryToVariant(this->messageId->getBinary().getByteArray());
        } else if (!this->messageId->getLongValue().isNull()) {
            object = AMQPSimpleType::longToVariant(this->messageId->getLongValue().toLongLong());
        } else if (!this->messageId->getString().isNull()) {
            object = AMQPSimpleType::stringToVariant(this->messageId->getString());
        } else if (!this->messageId->getUUID().isNull()) {
            object = AMQPSimpleType::uuidToVariant(this->messageId->getUUID());
        }
        list->addElementWithIndex(0, AMQPWrapper::wrapObject(object));
    }

    if (!this->userId.getByteArray().isNull()) {
        list->addElementWithIndex(1, AMQPWrapper::wrapBinary(this->userId.getByteArray()));
    }
    if (!this->to.isNull()) {
        list->addElementWithIndex(2, AMQPWrapper::wrapString(this->to));
    }
    if (!this->subject.isNull()) {
        list->addElementWithIndex(3, AMQPWrapper::wrapString(this->subject));
    }
    if (!this->replyTo.isNull()) {
        list->addElementWithIndex(4, AMQPWrapper::wrapString(this->replyTo));
    }
    if (!this->correlationID.getByteArray().isNull()) {
        list->addElementWithIndex(5, AMQPWrapper::wrapBinary(this->correlationID.getByteArray()));
    }
    if (!this->contentType.isNull()) {
        list->addElementWithIndex(6, AMQPWrapper::wrapString(this->contentType));
    }
    if (!this->contentEncoding.isNull()) {
        list->addElementWithIndex(7, AMQPWrapper::wrapString(this->contentEncoding));
    }
    if (!this->absoluteExpiryTime.isNull()) {
        list->addElementWithIndex(8, AMQPWrapper::wrapTimestamp(this->absoluteExpiryTime));
    }
    if (!this->creationTime.isNull()) {
        list->addElementWithIndex(9, AMQPWrapper::wrapTimestamp(this->creationTime));
    }
    if (!this->groupId.isNull()) {
        list->addElementWithIndex(10, AMQPWrapper::wrapString(this->groupId));
    }
    if (this->groupSequence != NULL) {
        list->addElementWithIndex(11, AMQPWrapper::wrapUInt(AMQPSimpleType::variantToUInt(this->groupSequence)));
    }
    if (!this->replyToGroupId.isNull()) {
        list->addElementWithIndex(12, AMQPWrapper::wrapString(this->replyToGroupId));
    }

    ByteArray data = ByteArray();
    data.writeChar(0x73);
    AMQPType *type = new AMQPType(AMQP_SMALL_ULONG_TYPE);
    AMQPTLVFixed *fixed = new AMQPTLVFixed(type, data);

    AMQPType *constructorType = new AMQPType(list->getTypeValue());
    AMQPDescribedConstructor *constructor = new AMQPDescribedConstructor(constructorType, fixed);

    list->setConstructor(constructor);
    return list;
}

AMQPSectionCode *AMQPProperties::getCode()
{
    return new AMQPSectionCode(AMQP_PROPERTIES_SECTION_CODE);
}

void AMQPProperties::fill(TLVAMQP *value)
{
    AMQPTLVList *list = new AMQPTLVList();

    if (list->getList().count() > 0) {
        TLVAMQP *element = list->getList().at(0);
        if (!element->isNull()) {
            switch (element->getTypeValue()) {
                case AMQP_ULONG0_TYPE:
                case AMQP_SMALL_ULONG_TYPE:
                case AMQP_ULONG_TYPE:
                {
                    long long value = AMQPUnwrapper::unwrapLong(element);
                    this->messageId = new AMQPLongID(QVariant(value));
                } break;
                case AMQP_STRING8_TYPE:
                case AMQP_STRING32_TYPE:
                {
                    this->messageId = new AMQPStringID(AMQPUnwrapper::unwrapString(element));
                } break;
                case AMQP_BINARY8_TYPE:
                case AMQP_BINARY32_TYPE:
                {
                    this->messageId = new AMQPBinaryID(ByteArray(AMQPUnwrapper::unwrapBinary(element)));
                } break;
                case AMQP_UUID_TYPE:
                {
                    this->messageId = new AMQPUUID(AMQPUnwrapper::unwrapUUID(element));
                } break;
                default: break;
            }
        }
    }
    if (list->getList().count() > 1) {
        TLVAMQP *element = list->getList().at(1);
        if (!element->isNull()) {
            this->userId = ByteArray(AMQPUnwrapper::unwrapBinary(element));
        }
    }
    if (list->getList().count() > 2) {
        TLVAMQP *element = list->getList().at(2);
        if (!element->isNull()) {
            this->to = AMQPUnwrapper::unwrapString(element);
        }
    }
    if (list->getList().count() > 3) {
        TLVAMQP *element = list->getList().at(3);
        if (!element->isNull()) {
            this->subject = AMQPUnwrapper::unwrapString(element);
        }
    }
    if (list->getList().count() > 4) {
        TLVAMQP *element = list->getList().at(4);
        if (!element->isNull()) {
            this->replyTo = AMQPUnwrapper::unwrapString(element);
        }
    }
    if (list->getList().count() > 5) {
        TLVAMQP *element = list->getList().at(5);
        if (!element->isNull()) {
            this->correlationID = AMQPUnwrapper::unwrapBinary(element);
        }
    }
    if (list->getList().count() > 6) {
        TLVAMQP *element = list->getList().at(6);
        if (!element->isNull()) {
            this->contentType = AMQPUnwrapper::unwrapString(element);
        }
    }
    if (list->getList().count() > 7) {
        TLVAMQP *element = list->getList().at(7);
        if (!element->isNull()) {
            this->contentEncoding = AMQPUnwrapper::unwrapString(element);
        }
    }
    if (list->getList().count() > 8) {
        TLVAMQP *element = list->getList().at(8);
        if (!element->isNull()) {
            this->absoluteExpiryTime = AMQPUnwrapper::unwrapTimestamp(element);
        }
    }
    if (list->getList().count() > 9) {
        TLVAMQP *element = list->getList().at(9);
        if (!element->isNull()) {
            this->creationTime = AMQPUnwrapper::unwrapTimestamp(element);
        }
    }
    if (list->getList().count() > 10) {
        TLVAMQP *element = list->getList().at(10);
        if (!element->isNull()) {
            this->groupId = AMQPUnwrapper::unwrapUInt(element);
        }
    }
    if (list->getList().count() > 11) {
        TLVAMQP *element = list->getList().at(11);
        if (!element->isNull()) {
            this->groupSequence = AMQPSimpleType::UIntToVariant(AMQPUnwrapper::unwrapUInt(element));
        }
    }
    if (list->getList().count() > 12) {
        TLVAMQP *element = list->getList().at(12);
        if (!element->isNull()) {
            this->replyToGroupId = AMQPUnwrapper::unwrapString(element);
        }
    }
}

AMQPMessageID *AMQPProperties::getMessageId() const
{
    return messageId;
}

void AMQPProperties::setMessageId(AMQPMessageID *value)
{
    messageId = value;
}

ByteArray AMQPProperties::getUserId() const
{
    return userId;
}

void AMQPProperties::setUserId(const ByteArray &value)
{
    userId = value;
}

QString AMQPProperties::getTo() const
{
    return to;
}

void AMQPProperties::setTo(const QString &value)
{
    to = value;
}

QString AMQPProperties::getSubject() const
{
    return subject;
}

void AMQPProperties::setSubject(const QString &value)
{
    subject = value;
}

QString AMQPProperties::getReplyTo() const
{
    return replyTo;
}

void AMQPProperties::setReplyTo(const QString &value)
{
    replyTo = value;
}

ByteArray AMQPProperties::getCorrelationID() const
{
    return correlationID;
}

void AMQPProperties::setCorrelationID(const ByteArray &value)
{
    correlationID = value;
}

QString AMQPProperties::getContentType() const
{
    return contentType;
}

void AMQPProperties::setContentType(const QString &value)
{
    contentType = value;
}

QString AMQPProperties::getContentEncoding() const
{
    return contentEncoding;
}

void AMQPProperties::setContentEncoding(const QString &value)
{
    contentEncoding = value;
}

QDateTime AMQPProperties::getAbsoluteExpiryTime() const
{
    return absoluteExpiryTime;
}

void AMQPProperties::setAbsoluteExpiryTime(const QDateTime &value)
{
    absoluteExpiryTime = value;
}

QDateTime AMQPProperties::getCreationTime() const
{
    return creationTime;
}

void AMQPProperties::setCreationTime(const QDateTime &value)
{
    creationTime = value;
}

QString AMQPProperties::getGroupId() const
{
    return groupId;
}

void AMQPProperties::setGroupId(const QString &value)
{
    groupId = value;
}

QVariant *AMQPProperties::getGroupSequence() const
{
    return groupSequence;
}

void AMQPProperties::setGroupSequence(QVariant *value)
{
    groupSequence = value;
}

QString AMQPProperties::getReplyToGroupId() const
{
    return replyToGroupId;
}

void AMQPProperties::setReplyToGroupId(const QString &value)
{
    replyToGroupId = value;
}
