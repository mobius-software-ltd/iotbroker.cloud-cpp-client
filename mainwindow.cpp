#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QBuffer>
#include <QByteArray>
#include <QDebug>
#include <QDir>
#include <QSqlQuery>
#include <QSqlDataBase>
#include <parser/messagesparser.h>
#include <QMessageBox>

// 198.41.30.241

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setStyleSheet("QMainWindow {background-image: url(:/resources/resources/iot_broker_background.jpg) }");
    this->setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowMinimizeButtonHint | Qt::WindowCloseButtonHint);

    this->mqtt = new MQTTManager(this);
    this->accountManager = AccountManager::getInstance();

    qDebug() << "Accounts number = " << this->accountManager->accounts().size();

    this->startNewSession();

    this->currentAccount = this->accountManager->readDefaultAccount();

    connect(ui->topicsTab, SIGNAL(addNewTopic(QString,int)), this, SLOT(willSubscribeSlot(QString,int)));
    connect(ui->topicsTab, SIGNAL(deleteTopic(QString,int)), this, SLOT(willUnsubscribeSlot(QString,int)));
    connect(ui->sendMessageTab, SIGNAL(donePublishForSending(Publish*)), this, SLOT(willPublishSlot(Publish*)));

    connect(this->mqtt,  SIGNAL(openConnection()),                  this, SLOT(openConnectionSlot()));
    connect(this->mqtt,  SIGNAL(closeConnection()),                 this, SLOT(closeConnectionSlot()));
    connect(this->mqtt,  SIGNAL(connackDidReceive(Connack*)),       this, SLOT(connackDidReceiveSlot(Connack*)));
    connect(this->mqtt,  SIGNAL(publishDidReceive(Publish*)),       this, SLOT(publishDidReceiveSlot(Publish*)));
    connect(this->mqtt,  SIGNAL(pubackDidReceive(Puback*)),         this, SLOT(pubackDidReceiveSlot(Puback*)));
    connect(this->mqtt,  SIGNAL(pubrecDidReceive(Pubrec*)),         this, SLOT(pubrecDidReceiveSlot(Pubrec*)));
    connect(this->mqtt,  SIGNAL(pubrelDidReceive(Pubrel*)),         this, SLOT(pubrelDidReceiveSlot(Pubrel*)));
    connect(this->mqtt,  SIGNAL(pubcompDidReceive(Pubcomp*)),       this, SLOT(pubcompDidReceiveSlot(Pubcomp*)));
    connect(this->mqtt,  SIGNAL(subackDidReceive(Suback*)),         this, SLOT(subackDidReceiveSlot(Suback*)));
    connect(this->mqtt,  SIGNAL(unsubackDidReceive(Unsuback*)),     this, SLOT(unsubackDidReceiveSlot(Unsuback*)));
    connect(this->mqtt,  SIGNAL(pingrespDidReceive(Pingresp*)),     this, SLOT(pingrespDidReceiveSlot(Pingresp*)));
    connect(this->mqtt,  SIGNAL(errorMessageDidReceive(QString*)),  this, SLOT(errorMessageDidReceiveSlot(QString*)));
    connect(this->mqtt,  SIGNAL(errorString(QString*)),             this, SLOT(errorSlot(QString*)));

    connect(ui->tabWidget, SIGNAL(currentChanged(int)), this, SLOT(logoutSlot(int)));

    ui->progressBar->setValue(0);

    this->progressTimer = new QTimer(this);

    connect(this->progressTimer, SIGNAL(timeout()), this, SLOT(timeoutProgressBar()));
    this->progressTimer->setInterval(32);

    this->timerMap = new TimersMap(this->mqtt);

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

    QString host =  this->currentAccount.serverHost.get().toString();
    int port = this->currentAccount.port.get().toInt();
    this->mqtt->openConnectionWith(host, port);
    qDebug() << host << " " << port;
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
    LoadingDialog *loading = new LoadingDialog();
    loading->setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowStaysOnTopHint);
    loading->show();
}

void MainWindow::saveTopic(Publish *publish, bool isIncoming)
{
    MessageEntity message;
    message.content = publish->getContent();
    message.qos = publish->getTopic()->getQoS()->getValue();
    message.topicName = publish->getTopic()->getName();
    this->accountManager->addMessageForDefaultAccount(message, isIncoming);

    ui->messagesListTab->setMessageList(this->accountManager->messagesForDefaultAccount());
}

//\//\//\//\//
//  SLOTS  //
//\//\//\///

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

void MainWindow::willSubscribeSlot(QString topicName, int qos)
{
    this->progressTimer->start();

    qDebug() << " !> " << topicName << " " << qos;

    QList<Topic> *topics = new QList<Topic>();
    topics->append(Topic(topicName, new QoS(qos)));

    Subscribe *subscribe = new Subscribe(topics);

    this->timerMap->goMessageTimer(subscribe);
}

void MainWindow::willUnsubscribeSlot(QString topicName, int qos)
{
    this->progressTimer->start();

    qDebug() << " !> " << topicName << " " << qos;

    QList<QString> *topics = new QList<QString>();
    topics->append(topicName);

    Unsubscribe *unsubscribe = new Unsubscribe(topics);

    this->timerMap->goMessageTimer(unsubscribe);
}

//

// Send message tab

void MainWindow::willPublishSlot(Publish *publish)
{

    qDebug() << "+--- " << "PUBLISH" << " ---";
    qDebug() << "| " << publish->getContent();
    qDebug() << "| " << publish->getTopic()->getName();
    qDebug() << "| " << publish->getTopic()->getQoS()->getValue();
    qDebug() << "| " << publish->isRetain();
    qDebug() << "| " << publish->isDup();
    qDebug() << "---  -------  ---";

    int qos = publish->getTopic()->getQoS()->getValue();
    if (qos != AT_MOST_ONCE) {
        this->progressTimer->start();
        this->timerMap->goMessageTimer(publish);
    } else {
        this->mqtt->publishMessage(publish);
    }
}

//

// MQTT MANAGER

void MainWindow::openConnectionSlot()
{
    qDebug() << " !> open";

    QString topicName = this->currentAccount.willTopic.get().toString();
    QByteArray byteArray =  this->currentAccount.will.get().toString().toUtf8();
    QoS *qos = new QoS(this->currentAccount.qos);
    Topic *topic = new Topic(topicName, qos);
    Will *will = new Will(topic, byteArray, this->currentAccount.isRetain);
    Connect *connect = new Connect(this->currentAccount.username, this->currentAccount.password, this->currentAccount.clientID, this->currentAccount.cleanSession, this->currentAccount.keepAlive, will);

    this->timerMap->goConnectTimer(connect);
}

void MainWindow::closeConnectionSlot()
{
    qDebug() << " !> close";

    this->accountManager->uncheckDefaultAccount();
}

void MainWindow::connackDidReceiveSlot(Connack *connack)
{
    qDebug() << " !> connack";

    if (connack->isValidReturnCode(connack->getReturnCode()) == true) {
        this->timerMap->stopConnectTimer();
        this->timerMap->goPingTimer(this->currentAccount.keepAlive);
    }
}

void MainWindow::publishDidReceiveSlot(Publish *publish)
{
    qDebug() << " !> publish";

    int qos = publish->getTopic()->getQoS()->getValue();
    if (qos == AT_LEAST_ONCE) {
        this->mqtt->pubackMessage(new Puback(publish->getPacketID()));
        this->saveTopic(publish, true);
    } else if (qos == EXACTLY_ONCE) {
        this->timerMap->goMessageTimer(new Pubrec(publish->getPacketID()));
        this->saveTopic(publish, true);
    }
}

void MainWindow::pubackDidReceiveSlot(Puback *puback)
{
    qDebug() << " !> puback";

    Message *message = this->timerMap->removeTimer(puback->getPacketID());
    if (message->getType() == PUBLISH) {
        Publish *publish = (Publish *)message;
        this->saveTopic(publish, false);
    }

    this->progressTimer->stop();
    ui->progressBar->setValue(0);
}

void MainWindow::pubrecDidReceiveSlot(Pubrec *pubrec)
{
    qDebug() << " !> pubrec";

    Message *message = this->timerMap->removeTimer(pubrec->getPacketID());
    if (message->getType() == PUBLISH) {
        Publish *publish = (Publish *)message;
        this->saveTopic(publish, false);
    }

    this->timerMap->goMessageTimer(new Pubrel(pubrec->getPacketID()));
}

void MainWindow::pubrelDidReceiveSlot(Pubrel *pubrel)
{
    qDebug() << " !> pubrel";

    this->timerMap->removeTimer(pubrel->getPacketID());

    this->mqtt->pubcompMessage(new Pubcomp(pubrel->getPacketID()));
}

void MainWindow::pubcompDidReceiveSlot(Pubcomp *pubcomp)
{
    qDebug() << " !> pubcomp";

    this->timerMap->removeTimer(pubcomp->getPacketID());

    this->progressTimer->stop();
    ui->progressBar->setValue(0);
}

void MainWindow::subackDidReceiveSlot(Suback *suback)
{
    qDebug() << " !> suback";

    Message *message = this->timerMap->removeTimer(suback->getPacketID());

    if (message->getType() == SUBSCRIBE) {
        Subscribe *subscribe = (Subscribe *)message;
        TopicEntity topic;
        topic.topicName = subscribe->getTopics()->at(0).getName();
        topic.qos = subscribe->getTopics()->at(0).getQoS()->getValue();
        this->accountManager->addTopicForDefaultAccount(topic);

        ui->topicsTab->setTopicsList(this->accountManager->topicsForDefaultAccount());
    }

    this->progressTimer->stop();
    ui->progressBar->setValue(0);
}

void MainWindow::unsubackDidReceiveSlot(Unsuback *unsuback)
{
    qDebug() << " !> unsuback";

    Message *message = this->timerMap->removeTimer(unsuback->getPacketID());

    if (message->getType() == UNSUBSCRIBE) {
        Unsubscribe *unsubscribe = (Unsubscribe *)message;
        QString topicName = unsubscribe->getTopics()->at(0);
        TopicEntity topic = this->accountManager->topicByName(topicName);
        this->accountManager->deleteTopic(topic);

        ui->topicsTab->setTopicsList(this->accountManager->topicsForDefaultAccount());
    }

    this->progressTimer->stop();
    ui->progressBar->setValue(0);
}

void MainWindow::pingrespDidReceiveSlot(Pingresp *pingresp)
{
    qDebug() << " !> pingresp";
}

void MainWindow::errorMessageDidReceiveSlot(QString *error)
{
    qDebug() << " !> error slot" << *error;

    QMessageBox *messageBox = new QMessageBox("Warrning", *error, QMessageBox::Warning, QMessageBox::Ok, QMessageBox::Cancel, QMessageBox::NoButton, this);
    messageBox->exec();
}

void MainWindow::errorSlot(QString *error)
{
    qDebug() << " !> error " << *error;
}

// QTabWidget

void MainWindow::logoutSlot(int tabIndex)
{
    int logoutIndex = ui->tabWidget->indexOf(ui->logoutTab);

    if (tabIndex == logoutIndex) {
        this->timerMap->stopAllTimers();
        this->mqtt->disconnectMessage();
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

MainWindow::~MainWindow()
{
    delete ui;
}
