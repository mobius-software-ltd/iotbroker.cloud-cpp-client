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

#include "cellaccountitem.h"
#include "ui_cellaccountitem.h"

CellAccountItem::CellAccountItem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CellAccountItem)
{
    ui->setupUi(this);
}

CellAccountItem *CellAccountItem::createCellWith(QString username, QString clientID, QString host, QString port, ListWidget *widget)
{
    CellAccountItem *widgetForm = new CellAccountItem();
    widgetForm->setUsername(username);
    widgetForm->setClientID(clientID);
    widgetForm->setHost(host);
    widgetForm->setPort(port);
    QListWidgetItem *item = new QListWidgetItem();
    item->setSizeHint(QSize(0,56));
    widget->addItem(item);
    widget->setItemWidget(item, widgetForm);
    widgetForm->setProperty("row", widget->row(item));
    return widgetForm;
}

QString CellAccountItem::getUsername()
{
    return ui->usernameLabel->text();
}

void CellAccountItem::setUsername(QString username)
{
    ui->usernameLabel->setText(username);
}

QString CellAccountItem::getClientID()
{
    return ui->clientIDLabel->text();
}

void CellAccountItem::setClientID(QString clientID)
{
    ui->clientIDLabel->setText(clientID);
}

QString CellAccountItem::getHost()
{
    return ui->hostLabel->text();
}

void CellAccountItem::setHost(QString host)
{
    ui->hostLabel->setText(host);
}

QString CellAccountItem::getPort()
{
    return ui->portLabel->text();
}

void CellAccountItem::setPort(QString port)
{
    ui->portLabel->setText(port);
}

void CellAccountItem::on_closeButton_clicked()
{
    int row = this->property("row").toInt();
    emit deleteButtonClickOn(row);
}

CellAccountItem::~CellAccountItem()
{
    delete ui;
}
