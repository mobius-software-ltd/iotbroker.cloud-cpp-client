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

#include "enumobject.h"

EnumObject::EnumObject(QObject *parent) : QObject(parent)
{
    this->map = QMap<int, QString>();
}

QMap<int, QString> EnumObject::getMap() const
{
    return this->map;
}

QString EnumObject::getName(int value)
{
    return this->map.value(value);
}

int EnumObject::getValue(QString name)
{
    for (int i = 0; i < this->map.count(); i++) {
        int key = this->map.keys().at(i);
        QString value = this->map.value(key);
        if (value == name) {
            return key;
        }
    }

    return UNKNOWN_ENUM_VALUE;
}
