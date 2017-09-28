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

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QByteArray>
#include <QDebug>
#include <QDir>
#include <QMessageBox>

// 198.41.30.241

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setStyleSheet("QMainWindow {background-image: url(:/resources/resources/iot_broker_background.jpg) }");
    this->setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowMinimizeButtonHint | Qt::WindowCloseButtonHint);

    this->accountManager = AccountManager::getInstance();
    this->currentAccount = this->accountManager->readDefaultAccount();

    connect(ui->topicsTab, SIGNAL(addNewTopic(TopicEntity)), this, SLOT(willSubscribeSlot(TopicEntity)));
    connect(ui->topicsTab, SIGNAL(deleteTopic(TopicEntity)), this, SLOT(willUnsubscribeSlot(TopicEntity)));
    connect(ui->sendMessageTab, SIGNAL(donePublishForSending(MessageEntity)), this, SLOT(willPublishSlot(MessageEntity)));
    connect(ui->tabWidget, SIGNAL(currentChanged(int)), this, SLOT(logoutSlot(int)));

    this->progressTimer = new QTimer(this);

    ui->progressBar->setValue(0);

    connect(this->progressTimer, SIGNAL(timeout()), this, SLOT(timeoutProgressBar()));
    this->progressTimer->setInterval(32);

    this->startNewSession();
    this->initMainWindow();
}

void MainWindow::startNewSession()
{
    DQList<AccountEntity> accounts = this->accountManager->accounts();

    if (accounts.size() > 0) {
        this->accountsListDialog = new AccountsListDialog();
        this->accountsListDialog->setAccountList(accounts);

        connect(this->accountsListDialog, SIGNAL(deleteAccount(AccountEntity*)),      this, SLOT(deleteAccountSlot(AccountEntity*)));
        connect(this->accountsListDialog, SIGNAL(accountDidSelect(AccountEntity*)),   this, SLOT(accountDidSelectSlot(AccountEntity*)));
        connect(this->accountsListDialog, SIGNAL(newAccountDidClick()),               this, SLOT(newAccountDidClickSlot()));

        if (this->accountsListDialog->exec() == 0) {
            QTimer::singleShot(0, this, SLOT(close()));
        }
    } else {
        if (showLogIn() == true) {
            this->setEnabled(false);
            this->showLoading();
        } else {
            QTimer::singleShot(0, this, SLOT(close()));
        }
    }
}

void MainWindow::initMainWindow()
{
    this->setWindowTitle(this->currentAccount.username);

    ui->topicsTab->setTopicsList(this->accountManager->topicsForDefaultAccount());
    ui->messagesListTab->setMessageList(this->accountManager->messagesForDefaultAccount());

    int protocolType = currentAccount.protocol.get().toInt();

    if (protocolType == MQTT_PROTOCOL) {
        this->iotProtocol = new MQTT(this->currentAccount);
    } else if (protocolType == MQTT_SN_PROTOCOL) {
        this->iotProtocol = new MqttSN(this->currentAccount);
    } else if (protocolType == COAP_PROTOCOL) {
        this->iotProtocol = new CoAP(this->currentAccount);
    } else if (protocolType == AMQP_PROTOCOL) {
        this->iotProtocol = new AMQP(this->currentAccount);
    }

    connect(this->iotProtocol, SIGNAL(connackReceived(IotProtocol*,int)),                                   this, SLOT(connackReceived(IotProtocol*,int)));
    connect(this->iotProtocol, SIGNAL(publishReceived(IotProtocol*,QString,int,QByteArray,bool,bool)),      this, SLOT(publishReceived(IotProtocol*,QString,int,QByteArray,bool,bool)));
    connect(this->iotProtocol, SIGNAL(pubackReceived(IotProtocol*,QString,int,QByteArray,bool,bool,int)),   this, SLOT(pubackReceived(IotProtocol*,QString,int,QByteArray,bool,bool,int)));
    connect(this->iotProtocol, SIGNAL(subackReceived(IotProtocol*,QString,int,int)),                        this, SLOT(subackReceived(IotProtocol*,QString,int,int)));
    connect(this->iotProtocol, SIGNAL(unsubackReceived(IotProtocol*,QString)),                              this, SLOT(unsubackReceived(IotProtocol*,QString)));
    connect(this->iotProtocol, SIGNAL(pingrespReceived(IotProtocol*)),                                      this, SLOT(pingrespReceived(IotProtocol*)));
    connect(this->iotProtocol, SIGNAL(disconnectReceived(IotProtocol*)),                                    this, SLOT(disconnectReceived(IotProtocol*)));
    connect(this->iotProtocol, SIGNAL(timeout(IotProtocol*)),                                               this, SLOT(timeout(IotProtocol*)));
    connect(this->iotProtocol, SIGNAL(errorReceived(IotProtocol*,QString)),                                 this, SLOT(errorReceived(IotProtocol*,QString)));

    this->iotProtocol->goConnect();
}

bool MainWindow::showLogIn()
{
    this->login = new LogInDialog();
    this->login->setWindowFlags (Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowMinimizeButtonHint | Qt::WindowCloseButtonHint);
    connect(this->login, SIGNAL(accountToSave(AccountEntity)), this, SLOT(saveNewAccount(AccountEntity)));
    return this->login->exec();
}

void MainWindow::showLoading()
{
    ui->tabWidget->tabBar()->setEnabled(false);
    this->loading = new LoadingDialog(this);
    this->loading->move(0, -10);
    this->loading->setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowStaysOnTopHint);
    this->loading->show();
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

    ui->messagesListTab->setMessageList(this->accountManager->messagesForDefaultAccount());
}

// -
// SLOTS
// -

// Accounts list dialog

void MainWindow::deleteAccountSlot(AccountEntity *account)
{
    this->accountManager->deleteAccount(*account);
    this->accountsListDialog->setAccountList(this->accountManager->accounts());
}

void MainWindow::accountDidSelectSlot(AccountEntity *account)
{
    this->accountsListDialog = NULL;
    this->accountManager->setDefaultAccountWithClientID(account->clientID);
    this->showLoading();
}

void MainWindow::newAccountDidClickSlot()
{
    this->accountsListDialog->close();
    this->accountsListDialog = NULL;

    if (showLogIn() == true) {
        this->showLoading();
    } else {
        QTimer::singleShot(0, this, SLOT(close()));
    }
}

//

//Account list dialog

void MainWindow::saveNewAccount(AccountEntity account)
{
    this->accountManager->addAccount(account);
    this->accountManager->setDefaultAccountWithClientID(account.clientID);
}

//

// Topic list tab

void MainWindow::willSubscribeSlot(TopicEntity topic)
{
    this->progressTimer->start();
    this->iotProtocol->subscribeTo(topic);
}

void MainWindow::willUnsubscribeSlot(TopicEntity topic)
{
    this->progressTimer->start();
    this->iotProtocol->unsubscribeFrom(topic);
}

//

// Send message tab

void MainWindow::willPublishSlot(MessageEntity message)
{
    this->iotProtocol->publish(message);
}

// QTabWidget

void MainWindow::logoutSlot(int tabIndex)
{
    int logoutIndex = ui->tabWidget->indexOf(ui->logoutTab);
    if (tabIndex == logoutIndex) {
        this->iotProtocol->disconnectWith(0);
        this->startNewSession();
        this->initMainWindow();
        ui->tabWidget->setCurrentIndex(1);
    }
}

//

// QProgressBar

void MainWindow::timeoutProgressBar()
{
    if ((ui->progressBar->value() < ui->progressBar->maximum() - 5) && ui->progressBar->value() >= ui->progressBar->minimum()) {
        ui->progressBar->setValue(ui->progressBar->value() + 1);
    }
}

//

// Iot protocol

void MainWindow::connackReceived(IotProtocol*,int)
{
    this->loading->accept();
    ui->tabWidget->tabBar()->setEnabled(true);
}

void MainWindow::publishReceived(IotProtocol*, QString topic, int qos, QByteArray content, bool isDub, bool isRetain)
{
    this->saveTopic(topic, qos, content, isRetain, isDub, true);
}

void MainWindow::pubackReceived(IotProtocol*, QString topic, int qos, QByteArray content, bool isDub, bool isRetain, int)
{
    this->saveTopic(topic, qos, content, isRetain, isDub, false);
    this->progressTimer->stop();
    ui->progressBar->setValue(0);
}

void MainWindow::subackReceived(IotProtocol*,QString topic,int qos,int)
{
    TopicEntity entity = TopicEntity();
    entity.topicName = topic;
    entity.qos = qos;

    this->accountManager->addTopicForDefaultAccount(entity);
    ui->topicsTab->setTopicsList(this->accountManager->topicsForDefaultAccount());

    this->progressTimer->stop();
    ui->progressBar->setValue(0);
}

void MainWindow::unsubackReceived(IotProtocol*,QString topic)
{
    this->accountManager->deleteTopic(this->accountManager->topicByName(topic));
    ui->topicsTab->setTopicsList(this->accountManager->topicsForDefaultAccount());

    this->progressTimer->stop();
    ui->progressBar->setValue(0);
}

void MainWindow::pingrespReceived(IotProtocol*)
{

}

void MainWindow::disconnectReceived(IotProtocol*)
{
    this->accountManager->uncheckDefaultAccount();
}

void MainWindow::timeout(IotProtocol*)
{
    QMessageBox *messageBox = new QMessageBox("Warrning", "Timeout", QMessageBox::Warning, QMessageBox::Ok, QMessageBox::Cancel, QMessageBox::NoButton, this);
    messageBox->setStyleSheet("QDialog {background-image: url(:/resources/resources/iot_broker_background.jpg) }");
    messageBox->exec();

    this->startNewSession();
    this->initMainWindow();
}

void MainWindow::errorReceived(IotProtocol*,QString error)
{
    QMessageBox *messageBox = new QMessageBox("Warrning", error, QMessageBox::Warning, QMessageBox::Ok, QMessageBox::Cancel, QMessageBox::NoButton, this);
    messageBox->setStyleSheet("QDialog {background-image: url(:/resources/resources/iot_broker_background.jpg) }");
    messageBox->exec();
}

//

MainWindow::~MainWindow()
{
    delete ui;
}
