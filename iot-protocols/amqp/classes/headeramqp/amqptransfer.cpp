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

#include "amqptransfer.h"
#include "iot-protocols/amqp/classes/constructors/amqpdescribedconstructor.h"
#include "iot-protocols/amqp/classes/tlv/fixed/amqptlvfixed.h"
#include "iot-protocols/amqp/classes/tlv/fixed/amqptlvnull.h"
#include "iot-protocols/amqp/classes/headerapi/amqpwrapper.h"
#include "iot-protocols/amqp/classes/headerapi/amqpunwrapper.h"
#include "iot-protocols/amqp/classes/headerapi/amqpfactory.h"
#include "iot-protocols/amqp/classes/section/amqpdata.h"
#include <QDebug>
#include <QObject>

AMQPTransfer::AMQPTransfer() : AMQPHeader(new AMQPHeaderCode(AMQP_TRANSFER_HEADER_CODE))
{
    this->sections = QMap<AMQPSectionCode *, AMQPSection *>();

    this->handle = NULL;
    this->deliveryId = NULL;
    this->deliveryTag = ByteArray();
    this->messageFormat = NULL;
    this->settled = NULL;
    this->more = NULL;
    this->rcvSettleMode = NULL;
    this->state = NULL;
    this->resume = NULL;
    this->aborted = NULL;
    this->batchable = NULL;
    this->sections = QMap<AMQPSectionCode *, AMQPSection *>();
}

int AMQPTransfer::getLength()
{
    int length = 8;
    AMQPTLVList *arguments = this->arguments();
    length += arguments->getLength();

    QList<AMQPSection *> array = this->sections.values();
    foreach (AMQPSection *item, array) {
        length += item->getValue()->getLength();
    }
    return length;
}

int AMQPTransfer::getType()
{
    return AMQP_TRANSFER_HEADER_CODE;
}

AMQPTLVList *AMQPTransfer::arguments()
{
    AMQPTLVList *list = new AMQPTLVList();

    if (this->handle == NULL) {
        qDebug() << "AMQPTransfer::arguments::handle";
        return new AMQPTLVList();
    }
    list->addElementWithIndex(0, AMQPWrapper::wrapUInt(AMQPSimpleType::variantToUInt(this->handle)));

    if (this->deliveryId != NULL) {
        list->addElementWithIndex(1, AMQPWrapper::wrapUInt(AMQPSimpleType::variantToUInt(this->deliveryId)));
    }
    if (!this->deliveryTag.getByteArray().isEmpty()) {
        list->addElementWithIndex(2, AMQPWrapper::wrapBinary(this->deliveryTag.getByteArray()));
    }
    if (this->messageFormat != NULL) {
        list->addElementWithIndex(3, AMQPWrapper::wrapUInt(this->messageFormat->encode()));
    }
    if (this->settled != NULL) {
        list->addElementWithIndex(4, AMQPWrapper::wrapBool(AMQPSimpleType::variantToBool(this->settled)));
    }
    if (this->more != NULL) {
        list->addElementWithIndex(5, AMQPWrapper::wrapBool(AMQPSimpleType::variantToBool(this->more)));
    }
    if (this->rcvSettleMode != NULL) {
        list->addElementWithIndex(6, AMQPWrapper::wrapUByte(this->rcvSettleMode->getValue()));
    }
    if (this->state != NULL) {
        list->addElementWithIndex(7, this->state->getList());
    }
    if (this->resume != NULL) {
        list->addElementWithIndex(8, AMQPWrapper::wrapBool(AMQPSimpleType::variantToBool(this->resume)));
    }
    if (this->aborted != NULL) {
        list->addElementWithIndex(9, AMQPWrapper::wrapBool(AMQPSimpleType::variantToBool(this->aborted)));
    }
    if (this->batchable != NULL) {
        list->addElementWithIndex(10, AMQPWrapper::wrapBool(AMQPSimpleType::variantToBool(this->batchable)));
    }
    ByteArray data = ByteArray();
    data.writeChar(this->code->getValue());
    AMQPType *type = new AMQPType(AMQP_SMALL_ULONG_TYPE);
    AMQPTLVFixed *fixed = new AMQPTLVFixed(type, data);

    AMQPType *constructorType = new AMQPType(list->getTypeValue());
    AMQPDescribedConstructor *constructor = new AMQPDescribedConstructor(constructorType, fixed);

    list->setConstructor(constructor);
    return list;
}

void AMQPTransfer::fillArguments(AMQPTLVList *list)
{
    int size = list->getList().count();

    if (size == 0) {
        qDebug() << "AMQPTransfer::fillArguments::size == 0";
        return;
    }
    if (size > 11) {
        qDebug() << "AMQPTransfer::fillArguments::wrong size";
        return;
    }

    if (size > 0) {
        TLVAMQP *element = list->getList().at(0);
        if (element->isNull()) {
            qDebug() << "AMQPTransfer::fillArguments::handle";
            return;
        }
        this->handle = AMQPSimpleType::UIntToVariant(AMQPUnwrapper::unwrapUInt(element));
    }
    if (size > 1) {
        TLVAMQP *element = list->getList().at(1);
        if (!element->isNull()) {
            this->deliveryId = AMQPSimpleType::UIntToVariant(AMQPUnwrapper::unwrapUInt(element));
        }
    }
    if (size > 2) {
        TLVAMQP *element = list->getList().at(2);
        if (!element->isNull()) {
            this->deliveryTag = ByteArray(AMQPUnwrapper::unwrapBinary(element));
        }
    }
    if (size > 3) {
        TLVAMQP *element = list->getList().at(3);
        if (!element->isNull()) {
            this->messageFormat = new AMQPMessageFormat(AMQPUnwrapper::unwrapUInt(element));
        }
    }
    if (size > 4) {
        TLVAMQP *element = list->getList().at(4);
        if (!element->isNull()) {
            this->settled = AMQPSimpleType::boolToVariant(AMQPUnwrapper::unwrapBool(element));
        }
    }
    if (size > 5) {
        TLVAMQP *element = list->getList().at(5);
        if (!element->isNull()) {
            this->more = AMQPSimpleType::boolToVariant(AMQPUnwrapper::unwrapBool(element));
        }
    }
    if (size > 6) {
        TLVAMQP *element = list->getList().at(6);
        if (!element->isNull()) {
            this->rcvSettleMode = new AMQPReceiverSettleMode(AMQPUnwrapper::unwrapUByte(element));
        }
    }
    if (size > 7) {
        TLVAMQP *element = list->getList().at(7);
        if (!element->isNull()) {
            if (element->getTypeValue() != AMQP_LIST0_TYPE && element->getTypeValue() != AMQP_LIST8_TYPE && element->getTypeValue() != AMQP_LIST32_TYPE) {
                qDebug() << "AMQPTransfer::fillArguments";
                return;
            }
            this->state = AMQPFactory::state((AMQPTLVList *)element);
            this->state->fill((AMQPTLVList *)element);
        }
    }
    if (size > 8) {
        TLVAMQP *element = list->getList().at(8);
        if (!element->isNull()) {
            this->resume = AMQPSimpleType::boolToVariant(AMQPUnwrapper::unwrapBool(element));
        }
    }
    if (size > 9) {
        TLVAMQP *element = list->getList().at(9);
        if (!element->isNull()) {
            this->aborted = AMQPSimpleType::boolToVariant(AMQPUnwrapper::unwrapBool(element));
        }
    }
    if (size > 10) {
        TLVAMQP *element = list->getList().at(10);
        if (!element->isNull()) {
            this->batchable = AMQPSimpleType::boolToVariant(AMQPUnwrapper::unwrapBool(element));
        }
    }
}

AMQPSection *AMQPTransfer::getHeader()
{
    AMQPSectionCode *sectionCode = new AMQPSectionCode(AMQP_HEADER_SECTION_CODE);
    return this->sections.value(sectionCode);
}

AMQPSection *AMQPTransfer::getDeliveryAnnotations()
{
    AMQPSectionCode *sectionCode = new AMQPSectionCode(AMQP_DELIVERY_ANNOTATIONS_SECTION_CODE);
    return this->sections.value(sectionCode);
}

AMQPSection *AMQPTransfer::getMessageAnnotations()
{
    AMQPSectionCode *sectionCode = new AMQPSectionCode(AMQP_MESSAGE_ANNOTATIONS_SECTION_CODE);
    return this->sections.value(sectionCode);
}

AMQPSection *AMQPTransfer::getProperties()
{
    AMQPSectionCode *sectionCode = new AMQPSectionCode(AMQP_PROPERTIES_SECTION_CODE);
    return this->sections.value(sectionCode);
}

AMQPSection *AMQPTransfer::getApplicationProperties()
{
    AMQPSectionCode *sectionCode = new AMQPSectionCode(AMQP_APPLICATION_PROPERTIES_SECTION_CODE);
    return this->sections.value(sectionCode);
}

AMQPSection *AMQPTransfer::getData()
{
    AMQPData *data = NULL;

    foreach (AMQPSection *item, this->sections.values()) {
        if (qobject_cast<AMQPData *>(item) != NULL) {
            data = (AMQPData *)item;
        }
    }
    return data;
}

AMQPSection *AMQPTransfer::getSequence()
{
    AMQPSectionCode *sectionCode = new AMQPSectionCode(AMQP_SEQUENCE_SECTION_CODE);
    return this->sections.value(sectionCode);
}

AMQPSection *AMQPTransfer::getValue()
{
    AMQPSectionCode *sectionCode = new AMQPSectionCode(AMQP_VALUE_SECTION_CODE);
    return this->sections.value(sectionCode);
}

AMQPSection *AMQPTransfer::getFooter()
{
    AMQPSectionCode *sectionCode = new AMQPSectionCode(AMQP_FOOTER_SECTION_CODE);
    return this->sections.value(sectionCode);
}

void AMQPTransfer::addSections(QList<AMQPSection *> array)
{
    foreach (AMQPSection *item, array) {
        this->sections.insert(item->getCode(), item);
    }
}



QVariant *AMQPTransfer::getHandle() const
{
    return handle;
}

void AMQPTransfer::setHandle(QVariant *value)
{
    handle = value;
}

QVariant *AMQPTransfer::getDeliveryId() const
{
    return deliveryId;
}

void AMQPTransfer::setDeliveryId(QVariant *value)
{
    deliveryId = value;
}

ByteArray AMQPTransfer::getDeliveryTag() const
{
    return deliveryTag;
}

void AMQPTransfer::setDeliveryTag(const ByteArray &value)
{
    deliveryTag = value;
}

AMQPMessageFormat *AMQPTransfer::getMessageFormat() const
{
    return messageFormat;
}

void AMQPTransfer::setMessageFormat(AMQPMessageFormat *value)
{
    messageFormat = value;
}

QVariant *AMQPTransfer::getSettled() const
{
    return settled;
}

void AMQPTransfer::setSettled(QVariant *value)
{
    settled = value;
}

QVariant *AMQPTransfer::getMore() const
{
    return more;
}

void AMQPTransfer::setMore(QVariant *value)
{
    more = value;
}

AMQPReceiverSettleMode *AMQPTransfer::getRcvSettleMode() const
{
    return rcvSettleMode;
}

void AMQPTransfer::setRcvSettleMode(AMQPReceiverSettleMode *value)
{
    rcvSettleMode = value;
}

AMQPState *AMQPTransfer::getState() const
{
    return state;
}

void AMQPTransfer::setState(AMQPState *value)
{
    state = value;
}

QVariant *AMQPTransfer::getResume() const
{
    return resume;
}

void AMQPTransfer::setResume(QVariant *value)
{
    resume = value;
}

QVariant *AMQPTransfer::getAborted() const
{
    return aborted;
}

void AMQPTransfer::setAborted(QVariant *value)
{
    aborted = value;
}

QVariant *AMQPTransfer::getBatchable() const
{
    return batchable;
}

void AMQPTransfer::setBatchable(QVariant *value)
{
    batchable = value;
}

QMap<AMQPSectionCode *, AMQPSection *> AMQPTransfer::getSections() const
{
    return sections;
}

void AMQPTransfer::setSections(const QMap<AMQPSectionCode *, AMQPSection *> &value)
{
    sections = value;
}
