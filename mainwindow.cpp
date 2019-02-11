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

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QByteArray>
#include <QDir>
#include <QMessageBox>
#include <QFileDialog>
#include <QDesktopWidget>
#include "iot-protocols/mqtt/mqtt.h"
#include "iot-protocols/mqtt-sn/mqttsn.h"
#include "iot-protocols/coap/coap.h"
#include "iot-protocols/amqp/classes/amqp.h"
#include "iot-protocols/websocket/websocketmqtt.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->progressTimer = new QTimer(this);
    connect(this->progressTimer, SIGNAL(timeout()), this, SLOT(timeoutProgressBar()));
    this->progressTimer->setInterval(32);

    this->accountManager = AccountManager::getInstance();
    this->init();
}

void MainWindow::init()
{
    if (this->accountManager->accounts().size() > 0) {
        this->accountListForm = new AccountListForm(ui->stackedWidget);
        this->accountListForm->setAccountList(this->accountManager->accounts());
        connect(this->accountListForm, SIGNAL(accountDidSelect(AccountEntity*)), this, SLOT(accountDidSelect(AccountEntity*)));
        connect(this->accountListForm, SIGNAL(deleteAccount(AccountEntity*)), this, SLOT(deleteAccount(AccountEntity*)));
        connect(this->accountListForm, SIGNAL(newAccountDidClick()), this, SLOT(newAccountDidClick()));
        ui->stackedWidget->addWidget(this->accountListForm);
        this->setSizeToWindowWithCentralPosition(this->accountListForm->getSize());
    } else {
        this->loginForm = new LoginForm(ui->stackedWidget);
        connect(this->loginForm, SIGNAL(securityKeyCellDidClick()), this, SLOT(securityKeyCellDidClick()));
        connect(this->loginForm, SIGNAL(accountToSave(AccountEntity)), this, SLOT(loginWithAccount(AccountEntity)));
        connect(this->loginForm, SIGNAL(needToResizeLoginForm(LoginForm*)), this, SLOT(needToResizeLoginForm(LoginForm*)));
        ui->stackedWidget->addWidget(this->loginForm);
        this->setSizeToWindowWithCentralPosition(this->loginForm->getSize());
    }
}

void MainWindow::startWithAccount(AccountEntity account)
{
    int protocolType = account.protocol.get().toInt();

    if (protocolType == MQTT_PROTOCOL) {
        this->iotProtocol = new MQTT(account);
    } else if (protocolType == MQTT_SN_PROTOCOL) {
        this->iotProtocol = new MqttSN(account);
    } else if (protocolType == COAP_PROTOCOL) {
        this->iotProtocol = new CoAP(account);
    } else if (protocolType == AMQP_PROTOCOL) {
        this->iotProtocol = new AMQP(account);
        QList<TopicEntity> topics = this->accountManager->topicsForAccount(account);
        ((AMQP *)this->iotProtocol)->setTopics(topics);
    } else if (protocolType == WEBSOCKET) {
        this->iotProtocol = new WebsocketMQTT(account);
    }

    connect(this->iotProtocol, SIGNAL(connackReceived(IotProtocol*,int)),                                   this, SLOT(connackReceived(IotProtocol*,int)));
    connect(this->iotProtocol, SIGNAL(publishReceived(IotProtocol*,QString,int,QByteArray,bool,bool)),      this, SLOT(publishReceived(IotProtocol*,QString,int,QByteArray,bool,bool)));
    connect(this->iotProtocol, SIGNAL(pubackReceived(IotProtocol*,QString,int,QByteArray,bool,bool,int)),   this, SLOT(pubackReceived(IotProtocol*,QString,int,QByteArray,bool,bool,int)));
    connect(this->iotProtocol, SIGNAL(subackReceived(IotProtocol*,QString,int,int)),                        this, SLOT(subackReceived(IotProtocol*,QString,int,int)));
    connect(this->iotProtocol, SIGNAL(unsubackReceived(IotProtocol*,QString)),                              this, SLOT(unsubackReceived(IotProtocol*,QString)));
    connect(this->iotProtocol, SIGNAL(disconnectReceived(IotProtocol*)),                                    this, SLOT(disconnectReceived(IotProtocol*)));
    connect(this->iotProtocol, SIGNAL(timeout(IotProtocol*)),                                               this, SLOT(timeout(IotProtocol*)));
    connect(this->iotProtocol, SIGNAL(errorReceived(IotProtocol*,QString)),                                 this, SLOT(errorReceived(IotProtocol*,QString)));

    this->iotProtocol->goConnect();
}

void MainWindow::setSizeToWindowWithCentralPosition(QSize size)
{
    this->setMinimumSize(size);
    this->setMaximumSize(size);
    QRect rect = this->geometry();
    rect.moveCenter(QApplication::desktop()->availableGeometry().center());
    this->setGeometry(rect);
}

void MainWindow::saveTopic(QString topicName, int qos, QByteArray content, bool isRetain, bool isDub, bool isIncoming)
{
    MessageEntity message;
    message.topicName = topicName;
    message.qos = qos;
    message.content = content;
    message.isRetain = isRetain;
    message.isDub = isDub;
    this->accountManager->addMessageForDefaultAccount(message, isIncoming);

    this->generalForm->setMessages(this->accountManager->messagesForDefaultAccount());
}

bool MainWindow::isTopicAlreadyExistForCurrentAccount(QString topic)
{
    QList<TopicEntity> topics = this->accountManager->topicsForDefaultAccount();
    for(int i = 0; i < topics.size(); i++) {
        if (topics.at(i).topicName == topic) {
            return true;
        }
    }
    return false;
}

// AccountListForm

void MainWindow::accountDidSelect(AccountEntity* account)
{
    this->accountManager->setDefaultAccountWithClientID(account->clientID);
    this->startWithAccount(this->accountManager->readDefaultAccount());

    if (account->protocol.get().toInt() != COAP_PROTOCOL) {
        ui->stackedWidget->removeWidget(ui->stackedWidget->currentWidget());

        this->loadingForm = new LoadingForm(ui->stackedWidget);
        ui->stackedWidget->addWidget(this->loadingForm);
        this->setSizeToWindowWithCentralPosition(this->loadingForm->getSize());
    }
}

void MainWindow::deleteAccount(AccountEntity* account)
{
    this->accountManager->deleteAccount(*account);
    this->accountListForm->setAccountList(this->accountManager->accounts());
}

void MainWindow::newAccountDidClick()
{
    ui->stackedWidget->removeWidget(ui->stackedWidget->currentWidget());

    this->loginForm = new LoginForm(ui->stackedWidget);
    connect(this->loginForm, SIGNAL(securityKeyCellDidClick()), this, SLOT(securityKeyCellDidClick()));
    connect(this->loginForm, SIGNAL(accountToSave(AccountEntity)), this, SLOT(loginWithAccount(AccountEntity)));
    connect(this->loginForm, SIGNAL(needToResizeLoginForm(LoginForm*)), this, SLOT(needToResizeLoginForm(LoginForm*)));
    ui->stackedWidget->addWidget(this->loginForm);
    this->setSizeToWindowWithCentralPosition(this->loginForm->getSize());
}

// GeneralForm

void MainWindow::willSubscribeToTopic(TopicEntity topicEntity)
{
    QString topicName = topicEntity.topicName.get().toString();
//    if (this->accountManager->isTopicExist(topicName)) {
//        QMessageBox *messageBox = new QMessageBox("Warrning", "Topic "+ topicName +" already exists.", QMessageBox::Warning, QMessageBox::Ok, QMessageBox::Cancel, QMessageBox::NoButton, this);
//        messageBox->setStyleSheet("QDialog {background-image: url(:/resources/resources/iot_broker_background.jpg) }");
//        messageBox->exec();
//    }
    this->progressTimer->start();
    this->iotProtocol->subscribeTo(topicEntity);
}

void MainWindow::willUnsubscribeFromTopic(TopicEntity topicEntity)
{
    this->progressTimer->start();
    this->iotProtocol->unsubscribeFrom(topicEntity);
}

void MainWindow::willPublish(MessageEntity message)
{
    int qos = message.qos.get().toInt();
    if (qos == AT_MOST_ONCE) {
        this->saveTopic(message.topicName.get().toString(), qos, message.content.get().toByteArray(), message.isRetain.get().toBool(), message.isDub.get().toBool(), false);
    } else {
        this->progressTimer->start();
    }
    this->iotProtocol->publish(message);
}

void MainWindow::topicsTabDidClick()
{
    this->generalForm->setTopics(this->accountManager->topicsForDefaultAccount());
}

void MainWindow::messagesTabDidCLick()
{
    this->generalForm->setMessages(this->accountManager->messagesForDefaultAccount());
}

void MainWindow::logoutTabDidCLick()
{
    ui->stackedWidget->removeWidget(ui->stackedWidget->currentWidget());
    this->iotProtocol->disconnectWith(0);
    this->accountManager->uncheckDefaultAccount();
    this->init();
}

// LoginForm

void MainWindow::loginWithAccount(AccountEntity account)
{
    if (this->accountManager->isAccountValid(account)) {
        if (this->accountManager->isAccountWithClientIdExist(account.clientID)) {
            QMessageBox *messageBox = new QMessageBox("Warning", "This client id have been already use", QMessageBox::Warning, QMessageBox::Ok, QMessageBox::Cancel, QMessageBox::NoButton, this);
            messageBox->setStyleSheet("QDialog {background-image: url(:/resources/resources/iot_broker_background.jpg) }");
            messageBox->exec();
            return;
        }

        int keepalive = account.keepAlive.get().toInt();
        if (keepalive <= 0 || keepalive > 65535) {
            QMessageBox *messageBox = new QMessageBox("Warrning", "Keepalive must be in the range [0, 65535].", QMessageBox::Warning, QMessageBox::Ok, QMessageBox::Cancel, QMessageBox::NoButton, this);
            messageBox->setStyleSheet("QDialog {background-image: url(:/resources/resources/iot_broker_background.jpg) }");
            messageBox->exec();
            return;
        }

        this->accountManager->addAccount(account);
        this->accountManager->setDefaultAccountWithClientID(account.clientID);
        this->startWithAccount(this->accountManager->readDefaultAccount());

        ui->stackedWidget->removeWidget(ui->stackedWidget->currentWidget());

        this->loadingForm = new LoadingForm(ui->stackedWidget);
        ui->stackedWidget->addWidget(this->loadingForm);
        this->setSizeToWindowWithCentralPosition(this->loadingForm->getSize());
    } else {
        QMessageBox *messageBox = new QMessageBox("Warning", "Please fill all fields", QMessageBox::Warning, QMessageBox::Ok, QMessageBox::Cancel, QMessageBox::NoButton, this);
        messageBox->setStyleSheet("QDialog {background-image: url(:/resources/resources/iot_broker_background.jpg) }");
        messageBox->exec();
    }

}

void MainWindow::securityKeyCellDidClick()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Get certificate file"), "", tr("All Files (*)"));
    this->loginForm->setKeyPath(fileName);
}

void MainWindow::needToResizeLoginForm(LoginForm *form)
{
    this->setSizeToWindowWithCentralPosition(form->getSize());
}

// Iot protocol

void MainWindow::connackReceived(IotProtocol *iotProtocol, int returnCode)
{
    Q_UNUSED(iotProtocol);
    if (returnCode == MQ_ACCEPTED) {
        ui->stackedWidget->removeWidget(ui->stackedWidget->currentWidget());

        this->generalForm = new GeneralForm(ui->stackedWidget);
        connect(this->generalForm, SIGNAL(willSubscribeToTopic(TopicEntity)), this, SLOT(willSubscribeToTopic(TopicEntity)));
        connect(this->generalForm, SIGNAL(willUnsubscribeFromTopic(TopicEntity)), this, SLOT(willUnsubscribeFromTopic(TopicEntity)));
        connect(this->generalForm, SIGNAL(willPublish(MessageEntity)), this, SLOT(willPublish(MessageEntity)));
        connect(this->generalForm, SIGNAL(topicsTabDidClick()), this, SLOT(topicsTabDidClick()));
        connect(this->generalForm, SIGNAL(messagesTabDidCLick()), this, SLOT(messagesTabDidCLick()));
        connect(this->generalForm, SIGNAL(logoutTabDidCLick()), this, SLOT(logoutTabDidCLick()));
        ui->stackedWidget->addWidget(this->generalForm);
        this->setSizeToWindowWithCentralPosition(this->generalForm->getSize());

        if (this->accountManager->readDefaultAccount().cleanSession.get().toBool()) {
            this->accountManager->removeTopicsForCurrentAccount();
        }
    }
}

void MainWindow::publishReceived(IotProtocol*, QString topic, int qos, QByteArray content, bool isDub, bool isRetain)
{
    this->saveTopic(topic, qos, content, isRetain, isDub, true);
}

void MainWindow::pubackReceived(IotProtocol*, QString topic, int qos, QByteArray content, bool isDub, bool isRetain, int)
{
    this->saveTopic(topic, qos, content, isRetain, isDub, false);
    this->progressTimer->stop();
    this->generalForm->setProgress(0);
}

void MainWindow::subackReceived(IotProtocol*,QString topic,int qos,int code)
{
    this->progressTimer->stop();
    this->generalForm->setProgress(0);

    if (code != 0) {
        QMessageBox *messageBox = new QMessageBox("Warrning", "Subscribe failure", QMessageBox::Warning, QMessageBox::Ok, QMessageBox::Cancel, QMessageBox::NoButton, this);
        messageBox->setStyleSheet("QDialog {background-image: url(:/resources/resources/iot_broker_background.jpg) }");
        messageBox->exec();
        return;
    }

    TopicEntity entity = TopicEntity();
    entity.topicName = topic;
    entity.qos = qos;

    this->accountManager->addTopicForDefaultAccount(entity);
    this->generalForm->setTopics(this->accountManager->topicsForDefaultAccount());
}

void MainWindow::unsubackReceived(IotProtocol*,QString topic)
{
    this->accountManager->deleteTopic(this->accountManager->topicByName(topic));
    this->generalForm->setTopics(this->accountManager->topicsForDefaultAccount());

    this->progressTimer->stop();
    this->generalForm->setProgress(0);
}

void MainWindow::disconnectReceived(IotProtocol*)
{
    ui->stackedWidget->removeWidget(ui->stackedWidget->currentWidget());
    this->accountManager->uncheckDefaultAccount();
    this->init();
}

void MainWindow::timeout(IotProtocol*)
{
    QMessageBox *messageBox = new QMessageBox("Warrning", "Timeout", QMessageBox::Warning, QMessageBox::Ok, QMessageBox::Cancel, QMessageBox::NoButton, this);
    messageBox->setStyleSheet("QDialog {background-image: url(:/resources/resources/iot_broker_background.jpg) }");
    messageBox->exec();
    this->logoutTabDidCLick();
}

void MainWindow::errorReceived(IotProtocol*,QString error)
{
    if (this->iotProtocol->getIsConnect()) {
        QMessageBox *messageBox = new QMessageBox("Warrning", error, QMessageBox::Warning, QMessageBox::Ok, QMessageBox::Cancel, QMessageBox::NoButton, this);
        messageBox->setStyleSheet("QDialog {background-image: url(:/resources/resources/iot_broker_background.jpg) }");
        messageBox->exec();
    }
}

// QProgressBar

void MainWindow::timeoutProgressBar()
{
    if ((this->generalForm->getProgress() < this->generalForm->getProgressMax() - 5) && this->generalForm->getProgress() >= this->generalForm->getProgressMin()) {
        this->generalForm->setProgress(this->generalForm->getProgress() + 1);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
