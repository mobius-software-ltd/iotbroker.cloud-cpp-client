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

#ifndef WILL_H
#define WILL_H

#include <QByteArray>
#include "iot-protocols/mqtt/classes/mqtopic.h"

/**
 * @brief The Will class
 */

class Will
{

private:
    MQTopic *topic;
    QByteArray content;
    bool retain;

public:
    Will();
    Will(MQTopic *topic, QByteArray content, bool retain);

    int retrieveLength();

    MQTopic *getTopic();
    void setTopic(MQTopic *topic);

    QByteArray getContent();
    void setContent(QByteArray content);

    bool isRetain();
    void setRetain(bool retain);

    bool isValid();
};

#endif // WILL_H
