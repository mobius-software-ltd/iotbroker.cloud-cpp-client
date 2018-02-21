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

#include "messageslisttab.h"
#include "ui_messageslisttab.h"

MessagesListTab::MessagesListTab(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MessagesListTab)
{
    ui->setupUi(this);
}

void MessagesListTab::addCell(QString topicName, QString content, bool isIncoming, int qos)
{
    CellMessageItem::createCellWith(topicName, content, isIncoming, qos, ui->listWidget);
}

void MessagesListTab::setMessageList(QList<MessageEntity> list)
{
    for (int i = 0 ; i < ui->listWidget->count(); i++) {
        QListWidgetItem *item = ui->listWidget->item(i);
        ui->listWidget->removeItemWidget(item);
    }
    ui->listWidget->clear();

    this->messages = list;

    for (int i = 0; i < this->messages.size(); i++) {
        MessageEntity item = this->messages.at(i);
        this->addCell(item.topicName, item.content.get().toString(), item.incoming, item.qos);
    }
}

MessagesListTab::~MessagesListTab()
{
    delete ui;
}
