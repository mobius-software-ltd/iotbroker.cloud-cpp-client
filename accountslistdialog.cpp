#include "accountslistdialog.h"
#include "ui_accountslistdialog.h"

AccountsListDialog::AccountsListDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AccountsListDialog)
{
    ui->setupUi(this);

    this->setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowMinimizeButtonHint | Qt::WindowCloseButtonHint);

    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(addButtonDidClick()));
    connect(ui->listWidget, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(rowDidSelect(QListWidgetItem*)));
}

void AccountsListDialog::addCell(QString name, QString clientID, QString host, int port)
{
    CellAccountItem *cell = CellAccountItem::createCellWith(name, clientID, host, QString::number(port), ui->listWidget);
    connect(cell, SIGNAL(deleteButtonClickOn(int)), this, SLOT(deleteItemAt(int)));
}

void AccountsListDialog::setAccountList(DQList<AccountEntity> list)
{
    for (int i = 0 ; i < ui->listWidget->count(); i++) {
        QListWidgetItem *item = ui->listWidget->item(i);
        ui->listWidget->removeItemWidget(item);
    }
    ui->listWidget->clear();

    this->accounts = list;

    for (int i = 0; i < this->accounts.size(); i++) {
        AccountEntity *item = this->accounts.at(i);
        this->addCell(item->username, item->clientID, item->serverHost, item->port);
    }
}

void AccountsListDialog::rowDidSelect(QListWidgetItem *item)
{
    int row = ui->listWidget->row(item);
    AccountEntity *accountItem = this->accounts.at(row);

    emit accountDidSelect(accountItem);
    accept();
}

void AccountsListDialog::addButtonDidClick()
{
    emit newAccountDidClick();
    accept();
}

void AccountsListDialog::deleteItemAt(int row)
{
    AccountEntity *item = this->accounts.at(row);

    emit deleteAccount(item);
}

AccountsListDialog::~AccountsListDialog()
{
    delete ui;
}
