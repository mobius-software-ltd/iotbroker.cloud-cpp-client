#include "topiclisttab.h"
#include "ui_topiclisttab.h"
#include <QMessageBox>

TopicListTab::TopicListTab(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TopicListTab)
{
    ui->setupUi(this);

    this->setStyleSheet("QDialog {background-image: url(:/resources/resources/iot_broker_background.jpg) }");

    connect(ui->addButton, SIGNAL(clicked()), this, SLOT(addButtonDidClick()));

    this->topicCell = CellWithEditLine::createCellWith(":/resources/resources/settings.png", "Topic:", "topic", ui->inputListWidget);
    this->qosCell = CellWithComboBox::createCellWith(":/resources/resources/settings.png", "QoS:", "0", ui->inputListWidget);
}

void TopicListTab::addCell(QString name, int qos)
{
    CellTopicItem *cell = CellTopicItem::createCellWith(name, qos, ui->listWidget);
    connect(cell, SIGNAL(deleteButtonClickOn(int)), this, SLOT(deleteItemAt(int)));
}

void TopicListTab::setTopicsList(QList<TopicEntity> list)
{
    for (int i = 0 ; i < ui->listWidget->count(); i++) {
        QListWidgetItem *item = ui->listWidget->item(i);
        ui->listWidget->removeItemWidget(item);
    }
    ui->listWidget->clear();

    this->topics = list;

    for (int i = 0; i < this->topics.size(); i++) {
        TopicEntity item = this->topics.at(i);
        this->addCell(item.topicName, item.qos);
    }
}

void TopicListTab::addButtonDidClick()
{
    QString topicName = this->topicCell->getInputText();
    int qos = this->qosCell->getValue().toInt();

    if (topicName.length() != 0) {
        emit addNewTopic(topicName, qos);
    } else {
        QMessageBox *messageBox = new QMessageBox("Warrning", "Please input topic name", QMessageBox::Warning, QMessageBox::Ok, QMessageBox::Cancel, QMessageBox::NoButton, this);
        messageBox->exec();
    }
}

void TopicListTab::deleteItemAt(int row)
{
    QListWidgetItem *item = ui->listWidget->item(row);
    CellTopicItem *widget = (CellTopicItem *)ui->listWidget->itemWidget(item);

    if (widget != NULL) {
        QString topicName = widget->getText();
        int qos = widget->getValue();
        emit deleteTopic(topicName, qos);
    }
}

TopicListTab::~TopicListTab()
{
    delete ui;
}
