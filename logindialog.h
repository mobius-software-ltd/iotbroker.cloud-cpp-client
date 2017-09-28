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

#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include <cells/cellwitheditline.h>
#include <cells/cellwithcheckbox.h>
#include <cells/cellwithcombobox.h>
#include <database/entities/accountentity.h>

namespace Ui {
class LogInDialog;
}

/**
 * @brief The LogInDialog class
 */

class LogInDialog : public QDialog
{
    Q_OBJECT

private:

    Ui::LogInDialog *ui;

    CellWithComboBox *protocolCell;

    CellWithEditLine *usernameCell;
    CellWithEditLine *passwordCell;
    CellWithEditLine *clientIDCell;
    CellWithEditLine *serverHostCell;
    CellWithEditLine *portCell;

    CellWithCheckbox *cleanSessionCell;
    CellWithEditLine *keepaliveCell;
    CellWithEditLine *willCell;
    CellWithEditLine *willTopicCell;
    CellWithCheckbox *retainCell;
    CellWithComboBox *qosCell;

    QList<QString> getInformation();
    bool isFieldsFill(QList<QString> list);

public:
    explicit LogInDialog(QWidget *parent = 0);
    ~LogInDialog();

signals:
    void accountToSave(AccountEntity account);

public slots:
    void logInButtonDidClick();
};

#endif // LOGINDIALOG_H
