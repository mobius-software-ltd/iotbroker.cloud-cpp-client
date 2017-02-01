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

#include "accountslistdialog.h"
#include "ui_accountslistdialog.h"

AccountsListDialog::AccountsListDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AccountsListDialog)
{
    ui->setupUi(this);

    this->setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowMinimizeButtonHint | Qt::WindowCloseButtonHint);

    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(addButtonDidClick()));
    connect(ui->listWidget, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(rowDidSelect(QListWidgetItem*)));
}

void AccountsListDialog::addCell(QString name, QString clientID, QString host, int port)
{
    CellAccountItem *cell = CellAccountItem::createCellWith(name, clientID, host, QString::number(port), ui->listWidget);
    connect(cell, SIGNAL(deleteButtonClickOn(int)), this, SLOT(deleteItemAt(int)));
}

void AccountsListDialog::setAccountList(DQList<AccountEntity> list)
{
    for (int i = 0 ; i < ui->listWidget->count(); i++) {
        QListWidgetItem *item = ui->listWidget->item(i);
        ui->listWidget->removeItemWidget(item);
    }
    ui->listWidget->clear();

    this->accounts = list;

    for (int i = 0; i < this->accounts.size(); i++) {
        AccountEntity *item = this->accounts.at(i);
        this->addCell(item->username, item->clientID, item->serverHost, item->port);
    }
}

void AccountsListDialog::rowDidSelect(QListWidgetItem *item)
{
    int row = ui->listWidget->row(item);
    AccountEntity *accountItem = this->accounts.at(row);

    emit accountDidSelect(accountItem);
    accept();
}

void AccountsListDialog::addButtonDidClick()
{
    emit newAccountDidClick();
    accept();
}

void AccountsListDialog::deleteItemAt(int row)
{
    AccountEntity *item = this->accounts.at(row);

    emit deleteAccount(item);
}

AccountsListDialog::~AccountsListDialog()
{
    delete ui;
}
