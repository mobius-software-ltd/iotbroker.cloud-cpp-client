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

#include "accountlistform.h"
#include "ui_accountlistform.h"
#include "iot-protocols/classes/iotenumprotocol.h"

AccountListForm::AccountListForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AccountListForm)
{
    ui->setupUi(this);

    this->setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowMinimizeButtonHint | Qt::WindowCloseButtonHint);

    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(addButtonDidClick()));
    connect(ui->listWidget, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(rowDidSelect(QListWidgetItem*)));
}

void AccountListForm::addCell(QString name, QString clientID, QString host, int port)
{
    CellAccountItem *cell = CellAccountItem::createCellWith(name, clientID, host, QString::number(port), ui->listWidget);
    connect(cell, SIGNAL(deleteButtonClickOn(int)), this, SLOT(deleteItemAt(int)));
}

void AccountListForm::setAccountList(DQList<AccountEntity> list)
{
    for (int i = 0 ; i < ui->listWidget->count(); i++) {
        QListWidgetItem *item = ui->listWidget->item(i);
        ui->listWidget->removeItemWidget(item);
    }
    ui->listWidget->clear();

    this->accounts = list;

    for (int i = 0; i < this->accounts.size(); i++) {
        AccountEntity *item = this->accounts.at(i);
        this->addCell(IotEnumProtocol(item->protocol).getName(), item->clientID, item->serverHost, item->port);
    }
}

void AccountListForm::rowDidSelect(QListWidgetItem *item)
{
    int row = ui->listWidget->row(item);
    AccountEntity *accountItem = this->accounts.at(row);

    emit accountDidSelect(accountItem);
}

void AccountListForm::addButtonDidClick()
{
    emit newAccountDidClick();
}

void AccountListForm::deleteItemAt(int row)
{
    AccountEntity *item = this->accounts.at(row);
    emit deleteAccount(item);
}

QSize AccountListForm::getSize()
{
    return QSize(300, 425);
}

AccountListForm::~AccountListForm()
{
    delete ui;
}
