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

#ifndef AMQPTLVLIST_H
#define AMQPTLVLIST_H

#include "iot-protocols/amqp/classes/tlv/api/tlvamqp.h"

class AMQPTLVList : public TLVAMQP
{
private:

    int width;
    int size;
    int count;

    QList<TLVAMQP *> list;

    void update();

public:
    AMQPTLVList();
    AMQPTLVList(AMQPType *type, QList<TLVAMQP *> value);

    virtual ByteArray getData();
    virtual int getLength();
    virtual ByteArray getValue();

    virtual QString description();

    void addElement(TLVAMQP *element);
    void setElementWithIndex(int index, TLVAMQP *element);
    void addElementWithIndex(int index, TLVAMQP *element);
    void addToListWithIndex(int index, int elementIndex, TLVAMQP *element);
    void addToMapWithIndex(int index, TLVAMQP *key, TLVAMQP *value);
    void addToArrayWithIndex(int index, TLVAMQP *element);

    int getWidth() const;
    int getSize() const;
    int getCount() const;
    QList<TLVAMQP *> getList() const;
};

#endif // AMQPTLVLIST_H
