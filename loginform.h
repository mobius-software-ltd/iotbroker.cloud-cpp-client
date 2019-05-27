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

#ifndef LOGINFORM_H
#define LOGINFORM_H

#include <QWidget>
#include <cells/cellwitheditline.h>
#include <cells/cellwithcheckbox.h>
#include <cells/cellwithcombobox.h>
#include <database/entities/accountentity.h>
#include "iot-protocols/classes/iotenumprotocol.h"
#include "iot-protocols/iotprotocol.h"

namespace Ui {
class LoginForm;
}

/**
 * @brief The LogInDialog class
 */

class LoginForm : public QWidget
{
    Q_OBJECT

    CellWithComboBox *protocolCell;

    CellWithEditLine *usernameCell;
    CellWithEditLine *passwordCell;
    CellWithEditLine *clientIDCell;
    CellWithEditLine *serverHostCell;
    CellWithEditLine *portCell;
    CellWithCheckbox *secureCell;

    CellWithCheckbox *cleanSessionCell;
    CellWithEditLine *keepaliveCell;
    CellWithEditLine *willCell;
    CellWithEditLine *willTopicCell;
    CellWithCheckbox *retainCell;
    CellWithComboBox *qosCell;

    CellWithEditLine *securityKeyCell;
    CellWithEditLine *keyPassword;


    QList<QString> getInformation();
    bool isFieldsFill(QList<QString> list);

    void showFieldsByProtocol(IotEnumProtocols protocol);

    int currentHeight;

public:
    explicit LoginForm(QWidget *parent = 0);
    ~LoginForm();

    QSize getSize();

    void setKeyPath(QString path);

private:
    Ui::LoginForm *ui;

signals:
    void accountToSave(AccountEntity account);
    void securityKeyCellDidClick();
    void needToResizeLoginForm(LoginForm *form);
    void timeout(IotProtocol*protocol);

public slots:
    void lineEditDidClick(QLineEdit *lineEdit);
    void logInButtonDidClick();
    void currentProtocolChanged(int protocol);
    void changeSecureState(bool state);
    void showMultilineWindow(bool value);
    void showWillMultilineWindow(bool value);
};

#endif // LOGINFORM_H
