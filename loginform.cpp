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

#include "loginform.h"
#include "ui_loginform.h"
#include "iot-protocols/classes/iotenumprotocol.h"
#include <QMessageBox>
#include <QDebug>

LoginForm::LoginForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LoginForm)
{
    ui->setupUi(this);

    this->setStyleSheet(".QFrame {background-image: url(:/resources/resources/iot_broker_background.jpg) }");

    QList<QString> protocolsList = QList<QString>();
    IotEnumProtocol *prot = new IotEnumProtocol();
    protocolsList.append(prot->EnumObject::getName(MQTT_PROTOCOL));
    protocolsList.append(prot->EnumObject::getName(MQTT_SN_PROTOCOL));
    protocolsList.append(prot->EnumObject::getName(COAP_PROTOCOL));
    protocolsList.append(prot->EnumObject::getName(AMQP_PROTOCOL));

    QList<QString> qosList = QList<QString>();
    qosList.append(QString::number(0));
    qosList.append(QString::number(1));
    qosList.append(QString::number(2));

    this->protocolCell      = CellWithComboBox::createCellWith(QString(":/resources/resources/settings.png"), "Protocol:", protocolsList, "protocol",     ui->registrationInfoWidget);

    this->usernameCell      = CellWithEditLine::createCellWith(QString(":/resources/resources/username.png"), "Username:",          "username",     ui->registrationInfoWidget);
    this->passwordCell      = CellWithEditLine::createCellWith(QString(":/resources/resources/password.png"), "Password:",          "password",     ui->registrationInfoWidget);
    this->clientIDCell      = CellWithEditLine::createCellWith(QString(":/resources/resources/clienid.png"),  "Client ID:",         "client id",    ui->registrationInfoWidget);
    this->serverHostCell    = CellWithEditLine::createCellWith(QString(":/resources/resources/host.png"),     "Server host:",       "server host",  ui->registrationInfoWidget);
    this->portCell          = CellWithEditLine::createCellWith(QString(":/resources/resources/host.png"),     "Port:",              "port",         ui->registrationInfoWidget);
    this->secureCell        = CellWithCheckbox::createCellWith(QString(":/resources/resources/settings.png"), "Secure connection:", false,          ui->registrationInfoWidget);

    this->cleanSessionCell  = CellWithCheckbox::createCellWith(QString(":/resources/resources/cleansession.png"), "Clean session:", false,          ui->settingsWidget);
    this->keepaliveCell     = CellWithEditLine::createCellWith(QString(":/resources/resources/keepalive.png"),    "Keepalive:",     "keepalive",    ui->settingsWidget);
    this->willCell          = CellWithEditLine::createCellWith(QString(":/resources/resources/settings.png"),     "Will:",          "will",         ui->settingsWidget);
    this->willTopicCell     = CellWithEditLine::createCellWith(QString(":/resources/resources/settings.png"),     "Will topic:",    "will topic",   ui->settingsWidget);
    this->retainCell        = CellWithCheckbox::createCellWith(QString(":/resources/resources/settings.png"),     "Retain:",        false,          ui->settingsWidget);
    this->qosCell           = CellWithComboBox::createCellWith(QString(":/resources/resources/settings.png"),     "QoS:",   qosList, "0",           ui->settingsWidget);

    this->securityKeyCell   = CellWithEditLine::createCellWith(QString(":/resources/resources/settings.png"),     "Security key:",  "click to select file", ui->securityWidget);
    this->keyPassword       = CellWithEditLine::createCellWith(QString(":/resources/resources/password.png"),     "Key password:",  "key password", ui->securityWidget);

    this->portCell->setNumbersValidator();
    this->keepaliveCell->setNumbersValidator();
    this->passwordCell->setPasswordMode(true);
    this->keyPassword->setPasswordMode(true);


    this->securityKeyCell->setLineEditClickFilter(true);
    QObject::connect(this->securityKeyCell, SIGNAL(didClick(QLineEdit*)), this, SLOT(lineEditDidClick(QLineEdit*)));

    QObject::connect(ui->pushButton, SIGNAL(clicked(bool)), this, SLOT(logInButtonDidClick()));
}

void LoginForm::lineEditDidClick(QLineEdit *lineEdit)
{
    emit securityKeyCellDidClick();
}


void LoginForm::logInButtonDidClick()
{
    QList<QString> list = this->getInformation();

    if (this->isFieldsFill(list) == false) {
        QMessageBox *messageBox = new QMessageBox("Warning", "Please fill all fields", QMessageBox::Warning, QMessageBox::Ok, QMessageBox::Cancel, QMessageBox::NoButton, this);
        messageBox->exec();

    } else {

        IotEnumProtocol *protocol = new IotEnumProtocol();

        AccountEntity account;
        account.protocol    = protocol->EnumObject::getValue(this->protocolCell->getValue());
        account.username    = this->usernameCell->getInputText();
        account.password    = this->passwordCell->getInputText();
        account.clientID    = this->clientIDCell->getInputText();
        account.serverHost  = this->serverHostCell->getInputText();
        account.port        = this->portCell->getInputText().toInt();
        account.cleanSession = this->cleanSessionCell->getState();
        account.keepAlive   = this->keepaliveCell->getInputText().toInt();
        account.will        = this->willCell->getInputText();
        account.willTopic   = this->willTopicCell->getInputText();
        account.isRetain    = this->retainCell->getState();
        account.qos         = this->qosCell->getValue().toInt();
        account.isDefault   = true;
        account.isSecure    = this->secureCell->getState();
        account.keyPath     = this->securityKeyCell->getInputText();
        account.keyPass     = this->keyPassword->getInputText();

        emit accountToSave(account);
    }
}

QList<QString> LoginForm::getInformation()
{
    QList<QString> list = QList<QString>();

    for (int i = 0; i < ui->registrationInfoWidget->count(); i++) {
        QListWidgetItem *item = ui->registrationInfoWidget->item(i);
        if (item != NULL) {
            QWidget *widget = ui->registrationInfoWidget->itemWidget(item);
            if (widget->metaObject()->className() == CellWithEditLine().metaObject()->className()) {
                CellWithEditLine *cell = (CellWithEditLine *)widget;
                list.append(cell->getInputText());
            }
        }
    }

    for (int i = 0; i < ui->settingsWidget->count(); i++) {
        QListWidgetItem *item = ui->settingsWidget->item(i);
        if (item != NULL) {
            QWidget *widget = ui->settingsWidget->itemWidget(item);
            if (widget->metaObject()->className() == CellWithEditLine().metaObject()->className()) {
                CellWithEditLine *cell = (CellWithEditLine *)widget;
                list.append(cell->getInputText());
            } else if (widget->metaObject()->className() == CellWithCheckbox().metaObject()->className()) {
                CellWithCheckbox *cell = (CellWithCheckbox *)widget;
                list.append(QString::number(cell->getState()));
            } else if (widget->metaObject()->className() == CellWithComboBox().metaObject()->className()) {
                CellWithComboBox *cell = (CellWithComboBox *)widget;
                list.append(cell->getValue());
            }
        }
    }

    return list;
}

bool LoginForm::isFieldsFill(QList<QString> list) {

    bool flag = true;

    for (int i = 0; i < list.size(); i++) {
        if (list.at(i).isEmpty()) {
            flag = false;
        }
    }
    return flag;
}

QSize LoginForm::getSize()
{
    return QSize(360, 690);
}

void LoginForm::setKeyPath(QString path)
{
    this->securityKeyCell->setInputText(path);
}

LoginForm::~LoginForm()
{
    delete ui;
}
