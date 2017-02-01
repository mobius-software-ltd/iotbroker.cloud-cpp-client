#include "cellaccountitem.h"
#include "ui_cellaccountitem.h"

CellAccountItem::CellAccountItem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CellAccountItem)
{
    ui->setupUi(this);
}

CellAccountItem *CellAccountItem::createCellWith(QString username, QString clientID, QString host, QString port, ListWidget *widget)
{
    CellAccountItem *widgetForm = new CellAccountItem();
    widgetForm->setUsername(username);
    widgetForm->setClientID(clientID);
    widgetForm->setHost(host);
    widgetForm->setPort(port);
    QListWidgetItem *item = new QListWidgetItem();
    item->setSizeHint(QSize(0,56));
    widget->addItem(item);
    widget->setItemWidget(item, widgetForm);
    widgetForm->setProperty("row", widget->row(item));
    return widgetForm;
}

QString CellAccountItem::getUsername()
{
    return ui->usernameLabel->text();
}

void CellAccountItem::setUsername(QString username)
{
    ui->usernameLabel->setText(username);
}

QString CellAccountItem::getClientID()
{
    return ui->clientIDLabel->text();
}

void CellAccountItem::setClientID(QString clientID)
{
    ui->clientIDLabel->setText(clientID);
}

QString CellAccountItem::getHost()
{
    return ui->hostLabel->text();
}

void CellAccountItem::setHost(QString host)
{
    ui->hostLabel->setText(host);
}

QString CellAccountItem::getPort()
{
    return ui->portLabel->text();
}

void CellAccountItem::setPort(QString port)
{
    ui->portLabel->setText(port);
}

void CellAccountItem::on_closeButton_clicked()
{
    int row = this->property("row").toInt();
    emit deleteButtonClickOn(row);
}

CellAccountItem::~CellAccountItem()
{
    delete ui;
}
