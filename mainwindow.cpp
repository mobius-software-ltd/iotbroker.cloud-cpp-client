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
    this->progressTimer->setInterval(50);

    this->accountManager = AccountManager::getInstance();
    this->init();
}
void MainWindow::closeEvent (QCloseEvent *event)
{
    if(this->loginForm != NULL && this->loginForm->isActiveWindow())
    {
       event->ignore();
       ui->stackedWidget->removeWidget(ui->stackedWidget->currentWidget());
       this->loginForm = NULL;
       this->init();
    }
    else
    {
        QMessageBox::StandardButton resBtn = QMessageBox::question( this, "IotBroker.cloud",
                                                                    tr("Are you sure?\n"),
                                                                    QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes);
        if (resBtn != QMessageBox::Yes) {
            event->ignore();
        } else {
            event->accept();
        }
    }
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
    this->accountEntity = account;
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
    this->accountManager->addMessageForAccount(this->accountEntity, message, isIncoming);

    this->generalForm->setMessages(this->accountManager->messagesForAccount(this->accountEntity));
}

bool MainWindow::isTopicAlreadyExistForCurrentAccount(QString topic)
{
    QList<TopicEntity> topics = this->accountManager->topicsForAccount(this->accountEntity);
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

    if (!(account->protocol.get().toInt() == COAP_PROTOCOL && !account->isSecure)) {
        ui->stackedWidget->removeWidget(ui->stackedWidget->currentWidget());

        this->loadingForm = new LoadingForm(ui->stackedWidget);
        connect(this->loadingForm, SIGNAL(timeout(IotProtocol*)), this, SLOT(timeout(IotProtocol*)));
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
    if(isTopicAlreadyExistForCurrentAccount(topicName) && this->accountEntity.protocol.get().toInt() == AMQP_PROTOCOL)
        return;

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

    int protocolType = this->accountEntity.protocol.get().toInt();
    if((protocolType == MQTT_SN_PROTOCOL || protocolType == COAP_PROTOCOL) && message.content.get().toString().size()>1399)
    {
        QMessageBox *messageBox = new QMessageBox("Warrning", "Max content size MUST be < 1400 characters for UDP", QMessageBox::Warning, QMessageBox::Ok, QMessageBox::Cancel, QMessageBox::NoButton, this);
        messageBox->setStyleSheet("QDialog {background-image: url(:/resources/resources/iot_broker_background.jpg) }");
        messageBox->exec();
        return;
    }

    int qos = message.qos.get().toInt();
    if (qos == AT_MOST_ONCE) {
        this->saveTopic(message.topicName.get().toString(), qos, message.content.get().toByteArray(), message.isRetain.get().toBool(), message.isDub.get().toBool(), false);
    }
    this->progressTimer->start();
    this->iotProtocol->publish(message);
}

void MainWindow::topicsTabDidClick()
{
    this->generalForm->setTopics(this->accountManager->topicsForAccount(this->accountEntity));
}

void MainWindow::messagesTabDidCLick()
{
    this->generalForm->setMessages(this->accountManager->messagesForAccount(this->accountEntity));
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

        int willSize = account.will.get().toString().size();
        if (account.protocol.get().toInt() == MQTT_SN_PROTOCOL && willSize > 1399 ) {
            QMessageBox *messageBox = new QMessageBox("Warrning", "Content of will message must be < 1400.", QMessageBox::Warning, QMessageBox::Ok, QMessageBox::Cancel, QMessageBox::NoButton, this);
            messageBox->setStyleSheet("QDialog {background-image: url(:/resources/resources/iot_broker_background.jpg) }");
            messageBox->exec();
            return;
        }

        if(account.isSecure.get().toBool() && account.keyPath != NULL && !account.keyPath.get().toString().isEmpty())
        {
            QString pem = account.keyPath;
            QByteArray * keyData = InternetProtocol::getKeyFromString(pem.toUtf8());
            pem.remove(*keyData,Qt::CaseSensitive);
            QByteArray keyPass = account.keyPass.get().toString().toUtf8();
            QString str = keyData->data();
            QSsl::KeyAlgorithm algo;
            if(str.startsWith(BEGINDSAKEYSTRING))
                algo = QSsl::KeyAlgorithm::Dsa;
             else
                algo = QSsl::KeyAlgorithm::Rsa;
            QSslKey sslKey = QSslKey(*keyData, algo, QSsl::EncodingFormat::Pem, QSsl::KeyType::PrivateKey, keyPass);
            if(sslKey.isNull())
            {
                QMessageBox *messageBox = new QMessageBox("Warrning", "Key of certificate is NULL. If kay has password check password", QMessageBox::Warning, QMessageBox::Ok, QMessageBox::Cancel, QMessageBox::NoButton, this);
                messageBox->setStyleSheet("QDialog {background-image: url(:/resources/resources/iot_broker_background.jpg) }");
                messageBox->exec();
                return;
            }
        }

        this->accountManager->addAccount(account);
        this->accountManager->setDefaultAccountWithClientID(account.clientID);
        this->startWithAccount(this->accountManager->readDefaultAccount());

        ui->stackedWidget->removeWidget(ui->stackedWidget->currentWidget());

        this->loadingForm = new LoadingForm(ui->stackedWidget);
        if (!(account.protocol.get().toInt() == COAP_PROTOCOL && !account.isSecure))
            connect(this->loadingForm, SIGNAL(timeout(IotProtocol*)), this, SLOT(timeout(IotProtocol*)));
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
    if (this->accountManager->readDefaultAccount().cleanSession.get().toBool()) {
        this->accountManager->removeTopicsForAccount(this->accountEntity);
    }

    Q_UNUSED(iotProtocol);
    if (returnCode == MQ_ACCEPTED) {
        //stop timer
        if(!(this->accountEntity.protocol.get().toInt() == COAP_PROTOCOL && !this->accountEntity.isSecure))
            this->loadingForm->stopTimer();
        ui->stackedWidget->removeWidget(ui->stackedWidget->currentWidget());

        this->generalForm = new GeneralForm(ui->stackedWidget);
        this->generalForm->setQoSForMessagesAndTopicsTab(this->accountEntity.protocol.get().toInt());
        this->generalForm->setTopics(this->accountManager->topicsForAccount(this->accountEntity));
        connect(this->generalForm, SIGNAL(willSubscribeToTopic(TopicEntity)), this, SLOT(willSubscribeToTopic(TopicEntity)));
        connect(this->generalForm, SIGNAL(willUnsubscribeFromTopic(TopicEntity)), this, SLOT(willUnsubscribeFromTopic(TopicEntity)));
        connect(this->generalForm, SIGNAL(willPublish(MessageEntity)), this, SLOT(willPublish(MessageEntity)));
        connect(this->generalForm, SIGNAL(topicsTabDidClick()), this, SLOT(topicsTabDidClick()));
        connect(this->generalForm, SIGNAL(messagesTabDidCLick()), this, SLOT(messagesTabDidCLick()));
        connect(this->generalForm, SIGNAL(logoutTabDidCLick()), this, SLOT(logoutTabDidCLick()));
        ui->stackedWidget->addWidget(this->generalForm);
        this->setSizeToWindowWithCentralPosition(this->generalForm->getSize());

    } else {
        QString stringError = "Connection failure. Error code : ";
        stringError.append(returnCode);
        QMessageBox *messageBox = new QMessageBox("Warrning", stringError, QMessageBox::Warning, QMessageBox::Ok, QMessageBox::Cancel, QMessageBox::NoButton, this);
        messageBox->setStyleSheet("QDialog {background-image: url(:/resources/resources/iot_broker_background.jpg) }");
        messageBox->exec();
    }
}

void MainWindow::publishReceived(IotProtocol*, QString topic, int qos, QByteArray content, bool isDub, bool isRetain)
{
    this->saveTopic(topic, qos, content, isRetain, isDub, true);
}

void MainWindow::pubackReceived(IotProtocol*, QString topic, int qos, QByteArray content, bool isDub, bool isRetain, int)
{
    this->saveTopic(topic, qos, content, isRetain, isDub, false);
}

void MainWindow::subackReceived(IotProtocol*,QString topic,int qos,int code)
{
    if (code != 0) {
        QMessageBox *messageBox = new QMessageBox("Warrning", "Subscribe failure", QMessageBox::Warning, QMessageBox::Ok, QMessageBox::Cancel, QMessageBox::NoButton, this);
        messageBox->setStyleSheet("QDialog {background-image: url(:/resources/resources/iot_broker_background.jpg) }");
        messageBox->exec();
        return;
    }

    TopicEntity entity = TopicEntity();
    entity.topicName = topic;
    entity.qos = qos;

    this->accountManager->addTopicForAccount(this->accountEntity, entity);
    this->generalForm->setTopics(this->accountManager->topicsForAccount(this->accountEntity));
}

void MainWindow::unsubackReceived(IotProtocol*,QString topic)
{
    this->accountManager->deleteTopic(this->accountManager->topicByName(topic));
    this->generalForm->setTopics(this->accountManager->topicsForAccount(this->accountEntity));
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
    QMessageBox *messageBox = new QMessageBox("Warrning", error, QMessageBox::Warning, QMessageBox::Ok, QMessageBox::Cancel, QMessageBox::NoButton, this);
    messageBox->setStyleSheet("QDialog {background-image: url(:/resources/resources/iot_broker_background.jpg) }");
    messageBox->exec();
    disconnectReceived(NULL);
    this->loadingForm->stopTimer();

}

// QProgressBar

void MainWindow::timeoutProgressBar()
{
    if ((this->generalForm->getProgress() < this->generalForm->getProgressMax()-5) && this->generalForm->getProgress() >= this->generalForm->getProgressMin()) {
        this->generalForm->setProgress(this->generalForm->getProgress() + 10);
    } else {
        this->progressTimer->stop();
        this->generalForm->setProgress(0);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
