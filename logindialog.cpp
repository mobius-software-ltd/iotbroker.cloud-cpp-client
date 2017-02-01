#include "logindialog.h"
#include "ui_logindialog.h"
#include <QMessageBox>

LogInDialog::LogInDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LogInDialog)
{
    ui->setupUi(this);

    this->setStyleSheet("QDialog {background-image: url(:/resources/resources/iot_broker_background.jpg) }");

    this->usernameCell      = CellWithEditLine::createCellWith(QString(":/resources/resources/username.png"), "Username:",      "username",     ui->registrationInfoWidget);
    this->passwordCell      = CellWithEditLine::createCellWith(QString(":/resources/resources/password.png"), "Password:",      "password",     ui->registrationInfoWidget);
    this->clientIDCell      = CellWithEditLine::createCellWith(QString(":/resources/resources/clienid.png"),  "Client ID:",     "client id",    ui->registrationInfoWidget);
    this->serverHostCell    = CellWithEditLine::createCellWith(QString(":/resources/resources/host.png"),     "Server host:",   "server host",  ui->registrationInfoWidget);
    this->portCell          = CellWithEditLine::createCellWith(QString(":/resources/resources/host.png"),     "Port:",          "port",         ui->registrationInfoWidget);

    this->cleanSessionCell  = CellWithCheckbox::createCellWith(QString(":/resources/resources/cleansession.png"), "Clean session:", false,          ui->settingsWidget);
    this->keepaliveCell     = CellWithEditLine::createCellWith(QString(":/resources/resources/keepalive.png"),    "Keepalive:",     "keepalive",    ui->settingsWidget);
    this->willCell          = CellWithEditLine::createCellWith(QString(":/resources/resources/settings.png"),     "Will:",          "will",         ui->settingsWidget);
    this->willTopicCell     = CellWithEditLine::createCellWith(QString(":/resources/resources/settings.png"),     "Will topic:",    "will topic",   ui->settingsWidget);
    this->retainCell        = CellWithCheckbox::createCellWith(QString(":/resources/resources/settings.png"),     "Retain:",        false,          ui->settingsWidget);
    this->qosCell           = CellWithComboBox::createCellWith(QString(":/resources/resources/settings.png"),     "QoS:",           "0",            ui->settingsWidget);

    connect(ui->pushButton, SIGNAL(clicked(bool)), this, SLOT(logInButtonDidClick()));
}

void LogInDialog::logInButtonDidClick()
{
    QList<QString> list = this->getInformation();

    if (this->isFieldsFill(list) == false) {
        QMessageBox *messageBox = new QMessageBox("Warning", "Please fill all fields", QMessageBox::Warning, QMessageBox::Ok, QMessageBox::Cancel, QMessageBox::NoButton, this);
        messageBox->exec();

    } else {
        AccountEntity account;
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
        account.qos         = this->qosCell->getText().toInt();
        account.isDefault = true;

        qDebug() << account;

        emit accountToSave(account);
        accept();
    }
}

QList<QString> LogInDialog::getInformation()
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

bool LogInDialog::isFieldsFill(QList<QString> list) {

    bool flag = true;

    for (int i = 0; i < list.size(); i++) {
        if (list.at(i).isEmpty()) {
            flag = false;
        }
    }
    return flag;
}

LogInDialog::~LogInDialog()
{
    delete ui;
}
