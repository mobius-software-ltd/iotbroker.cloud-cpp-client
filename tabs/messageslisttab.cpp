#include "messageslisttab.h"
#include "ui_messageslisttab.h"

MessagesListTab::MessagesListTab(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MessagesListTab)
{
    ui->setupUi(this);
}

void MessagesListTab::addCell(QString topicName, QString content, bool isIncoming, int qos)
{
    CellMessageItem::createCellWith(topicName, content, isIncoming, qos, ui->listWidget);
}

void MessagesListTab::setMessageList(QList<MessageEntity> list)
{
    for (int i = 0 ; i < ui->listWidget->count(); i++) {
        QListWidgetItem *item = ui->listWidget->item(i);
        ui->listWidget->removeItemWidget(item);
    }
    ui->listWidget->clear();

    this->messages = list;

    for (int i = 0; i < this->messages.size(); i++) {
        MessageEntity item = this->messages.at(i);
        this->addCell(item.topicName, item.content.get().toString(), item.incoming, item.qos);
    }
}

MessagesListTab::~MessagesListTab()
{
    delete ui;
}
