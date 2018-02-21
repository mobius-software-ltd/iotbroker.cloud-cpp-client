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

#ifndef MESSAGESLISTTAB_H
#define MESSAGESLISTTAB_H

#include <QWidget>
#include <cells/cellmessageitem.h>
#include <database/entities/messageentity.h>

namespace Ui {
class MessagesListTab;
}

/**
 * @brief The MessagesListTab class
 */

class MessagesListTab : public QWidget
{
    Q_OBJECT

private:

    Ui::MessagesListTab *ui;

    QList<MessageEntity> messages;

    void addCell(QString topicName, QString content, bool isIncoming, int qos);

public:
    explicit MessagesListTab(QWidget *parent = 0);
    ~MessagesListTab();

    void setMessageList(QList<MessageEntity> list);
};

#endif // MESSAGESLISTTAB_H
