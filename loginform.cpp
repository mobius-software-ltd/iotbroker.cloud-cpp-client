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
#include <QMessageBox>
#include "mainwindow.h"

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
    protocolsList.append(prot->EnumObject::getName(WEBSOCKET));

    QList<QString> qosList = QList<QString>();
    qosList.append(QString::number(0));
    qosList.append(QString::number(1));
    qosList.append(QString::number(2));

    this->protocolCell      = CellWithComboBox::createCellWith(QString(":/resources/resources/settings.png"), "Protocol:", protocolsList, "protocol",     ui->registrationInfoWidget);
    connect(this->protocolCell->getComboBox(), SIGNAL(currentIndexChanged(int)), this, SLOT(currentProtocolChanged(int)));

    this->usernameCell      = CellWithEditLine::createCellWith(QString(":/resources/resources/username.png"), "Username:",          "username",     ui->registrationInfoWidget);
    this->passwordCell      = CellWithEditLine::createCellWith(QString(":/resources/resources/password.png"), "Password:",          "password",     ui->registrationInfoWidget);
    this->clientIDCell      = CellWithEditLine::createCellWith(QString(":/resources/resources/clienid.png"),  "Client ID:",         "client id",    ui->registrationInfoWidget);
    this->serverHostCell    = CellWithEditLine::createCellWith(QString(":/resources/resources/host.png"),     "Server host:",       "server host",  ui->registrationInfoWidget);
    this->portCell          = CellWithEditLine::createCellWith(QString(":/resources/resources/host.png"),     "Port:",              "port",         ui->registrationInfoWidget);
    this->secureCell        = CellWithCheckbox::createCellWith(QString(":/resources/resources/settings.png"), "Secure connection:", false,          ui->registrationInfoWidget);
    connect(this->secureCell->getCheckBox(), SIGNAL(clicked(bool)), this, SLOT(changeSecureState(bool)));

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


    this->showFieldsByProtocol(MQTT_PROTOCOL);
}

void LoginForm::lineEditDidClick(QLineEdit *lineEdit)
{
    Q_UNUSED(lineEdit);
    emit securityKeyCellDidClick();
}


void LoginForm::logInButtonDidClick()
{
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

void LoginForm::currentProtocolChanged(int p)
{
    IotEnumProtocols protocol = (IotEnumProtocols)(p + 1);
    this->showFieldsByProtocol(protocol);
    emit needToResizeLoginForm(this);
}

void LoginForm::changeSecureState(bool state)
{
    int cellHeight = this->protocolCell->size().height() - 2;

    if (state) {
        this->currentHeight += cellHeight;
        this->currentHeight += cellHeight;
        this->currentHeight += ui->label_3->height();

        ui->label_3->setHidden(false);
        ui->securityWidget->setHidden(false);
    } else {
        this->currentHeight -= cellHeight;
        this->currentHeight -= cellHeight;
        this->currentHeight -= ui->label_3->height();

        ui->label_3->setHidden(true);
        ui->securityWidget->setHidden(true);
    }
    emit needToResizeLoginForm(this);
}

void LoginForm::showFieldsByProtocol(IotEnumProtocols protocol)
{
    this->currentHeight = 690;
    int registrationInfoWidgetHeight = 270;
    int settingsWidgetHeight = 230;

    int cellHeight = this->protocolCell->size().height() - 2;

    if (protocol == MQTT_SN_PROTOCOL) {

        ui->registrationInfoWidget->item(1) ->setHidden(true);
        ui->registrationInfoWidget->item(2)->setHidden(true);
        ui->registrationInfoWidget->item(3)->setHidden(false);
        ui->registrationInfoWidget->item(4)->setHidden(false);
        ui->registrationInfoWidget->item(5)->setHidden(false);
        ui->registrationInfoWidget->item(6)->setHidden(false);

        registrationInfoWidgetHeight -= cellHeight;
        registrationInfoWidgetHeight -= cellHeight;

        this->currentHeight -= cellHeight;
        this->currentHeight -= cellHeight;

        ui->settingsWidget->item(0)->setHidden(false);
        ui->settingsWidget->item(1)->setHidden(false);
        ui->settingsWidget->item(2)->setHidden(false);
        ui->settingsWidget->item(3)->setHidden(false);
        ui->settingsWidget->item(4)->setHidden(false);
        ui->settingsWidget->item(5)->setHidden(false);

    } else if (protocol == COAP_PROTOCOL) {

        ui->registrationInfoWidget->item(1) ->setHidden(true);
        ui->registrationInfoWidget->item(2)->setHidden(true);
        ui->registrationInfoWidget->item(3)->setHidden(false);
        ui->registrationInfoWidget->item(4)->setHidden(false);
        ui->registrationInfoWidget->item(5)->setHidden(false);
        ui->registrationInfoWidget->item(6)->setHidden(false);

        registrationInfoWidgetHeight -= cellHeight;
        registrationInfoWidgetHeight -= cellHeight;

        this->currentHeight -= cellHeight;
        this->currentHeight -= cellHeight;

        ui->settingsWidget->item(0)->setHidden(true);
        ui->settingsWidget->item(1)->setHidden(false);
        ui->settingsWidget->item(2)->setHidden(true);
        ui->settingsWidget->item(3)->setHidden(true);
        ui->settingsWidget->item(4)->setHidden(true);
        ui->settingsWidget->item(5)->setHidden(true);

        settingsWidgetHeight -= cellHeight;
        settingsWidgetHeight -= cellHeight;
        settingsWidgetHeight -= cellHeight;
        settingsWidgetHeight -= cellHeight;
        settingsWidgetHeight -= cellHeight;

        this->currentHeight -= cellHeight;
        this->currentHeight -= cellHeight;
        this->currentHeight -= cellHeight;
        this->currentHeight -= cellHeight;
        this->currentHeight -= cellHeight;

    } else if (protocol == AMQP_PROTOCOL) {

        ui->registrationInfoWidget->item(1) ->setHidden(false);
        ui->registrationInfoWidget->item(2)->setHidden(false);
        ui->registrationInfoWidget->item(3)->setHidden(false);
        ui->registrationInfoWidget->item(4)->setHidden(false);
        ui->registrationInfoWidget->item(5)->setHidden(false);
        ui->registrationInfoWidget->item(6)->setHidden(false);

        ui->settingsWidget->item(0)->setHidden(true);
        ui->settingsWidget->item(1)->setHidden(false);
        ui->settingsWidget->item(2)->setHidden(true);
        ui->settingsWidget->item(3)->setHidden(true);
        ui->settingsWidget->item(4)->setHidden(true);
        ui->settingsWidget->item(5)->setHidden(true);

        settingsWidgetHeight -= cellHeight;
        settingsWidgetHeight -= cellHeight;
        settingsWidgetHeight -= cellHeight;
        settingsWidgetHeight -= cellHeight;
        settingsWidgetHeight -= cellHeight;

        this->currentHeight -= cellHeight;
        this->currentHeight -= cellHeight;
        this->currentHeight -= cellHeight;
        this->currentHeight -= cellHeight;
        this->currentHeight -= cellHeight;

    } else {

        ui->registrationInfoWidget->item(1) ->setHidden(false);
        ui->registrationInfoWidget->item(2)->setHidden(false);
        ui->registrationInfoWidget->item(3)->setHidden(false);
        ui->registrationInfoWidget->item(4)->setHidden(false);
        ui->registrationInfoWidget->item(5)->setHidden(false);
        ui->registrationInfoWidget->item(6)->setHidden(false);

        ui->settingsWidget->item(0)->setHidden(false);
        ui->settingsWidget->item(1)->setHidden(false);
        ui->settingsWidget->item(2)->setHidden(false);
        ui->settingsWidget->item(3)->setHidden(false);
        ui->settingsWidget->item(4)->setHidden(false);
        ui->settingsWidget->item(5)->setHidden(false);

    }

    ui->registrationInfoWidget->setMinimumHeight(registrationInfoWidgetHeight);
    ui->settingsWidget->setMinimumHeight(settingsWidgetHeight);

    if (this->secureCell->getState()) {
        ui->label_3->setHidden(false);
        ui->securityWidget->setHidden(false);
    } else {
        this->currentHeight -= cellHeight;
        this->currentHeight -= cellHeight;
        this->currentHeight -= ui->label_3->height();

        ui->label_3->setHidden(true);
        ui->securityWidget->setHidden(true);
    }

}

QSize LoginForm::getSize()
{
    return QSize(360, this->currentHeight);
}

void LoginForm::setKeyPath(QString path)
{
    this->securityKeyCell->setInputText(path);
}

LoginForm::~LoginForm()
{
    delete ui;
}
