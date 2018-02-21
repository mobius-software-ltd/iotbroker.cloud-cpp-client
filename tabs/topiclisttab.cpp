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

    QList<QString> qosList = QList<QString>();
    qosList.append(QString::number(0));
    qosList.append(QString::number(1));
    qosList.append(QString::number(2));

    this->topicCell = CellWithEditLine::createCellWith(":/resources/resources/settings.png", "Topic:", "topic", ui->inputListWidget);
    this->qosCell = CellWithComboBox::createCellWith(":/resources/resources/settings.png", "QoS:", qosList, "0", ui->inputListWidget);
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

    TopicEntity topic = TopicEntity();
    topic.topicName = topicName;
    topic.qos = this->qosCell->getValue().toInt();

    if (topicName.length() != 0) {
        emit addNewTopic(topic);
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
        TopicEntity topic = TopicEntity();
        topic.topicName = widget->getText();
        topic.qos = widget->getValue();
        emit deleteTopic(topic);
    }
}

TopicListTab::~TopicListTab()
{
    delete ui;
}
