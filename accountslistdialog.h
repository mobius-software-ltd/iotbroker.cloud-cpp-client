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

#ifndef ACCOUNTSLISTDIALOG_H
#define ACCOUNTSLISTDIALOG_H

#include <QDialog>
#include <cells/cellaccountitem.h>
#include <database/entities/accountentity.h>

namespace Ui {
class AccountsListDialog;
}

/**
 * @brief The AccountsListDialog class
 */

class AccountsListDialog : public QDialog
{
    Q_OBJECT

private:

    Ui::AccountsListDialog *ui;

    DQList<AccountEntity> accounts;

    void addCell(QString name, QString clientID, QString host, int port);

public:
    explicit AccountsListDialog(QWidget *parent = 0);
    ~AccountsListDialog();

    void setAccountList(DQList<AccountEntity> list);

signals:
    void deleteAccount(AccountEntity *account);
    void accountDidSelect(AccountEntity *account);
    void newAccountDidClick();

private slots:
    void rowDidSelect(QListWidgetItem* item);
    void addButtonDidClick();
    void deleteItemAt(int row);
};

#endif // ACCOUNTSLISTDIALOG_H
