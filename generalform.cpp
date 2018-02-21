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

#include "generalform.h"
#include "ui_generalform.h"
#include <QDebug>

GeneralForm::GeneralForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GeneralForm)
{
    ui->setupUi(this);

    this->setStyleSheet(".QFrame {background-image: url(:/resources/resources/iot_broker_background.jpg) }");

    ui->progressBar->setValue(0);

    connect(ui->topicsTab, SIGNAL(addNewTopic(TopicEntity)), this, SLOT(willSubscribeToTopicSlot(TopicEntity)));
    connect(ui->topicsTab, SIGNAL(deleteTopic(TopicEntity)), this, SLOT(willUnsubscribeFromTopicSlot(TopicEntity)));
    connect(ui->sendMessageTab, SIGNAL(donePublishForSending(MessageEntity)), this, SLOT(willPublishSlot(MessageEntity)));
    connect(ui->tabWidget, SIGNAL(currentChanged(int)), this, SLOT(currentTabChangedSlot(int)));
    connect(ui->tabWidget, SIGNAL(tabBarClicked(int)), this, SLOT(tabBarClicked()));
}

void GeneralForm::willSubscribeToTopicSlot(TopicEntity topicEntity)
{
    emit willSubscribeToTopic(topicEntity);
}

void GeneralForm::willUnsubscribeFromTopicSlot(TopicEntity topicEntity)
{
    emit willUnsubscribeFromTopic(topicEntity);
}

void GeneralForm::willPublishSlot(MessageEntity message)
{
    emit willPublish(message);
}

void GeneralForm::currentTabChangedSlot(int index)
{
    int topicsTabIndex = ui->tabWidget->indexOf(ui->topicsTab);
    int messagesTabIndex = ui->tabWidget->indexOf(ui->messagesListTab);
    int logoutTabIndex = ui->tabWidget->indexOf(ui->logoutTab);

    if (index == topicsTabIndex) {
        emit topicsTabDidClick();
    } else if (index == messagesTabIndex) {
        emit messagesTabDidCLick();
    } else if (index == logoutTabIndex) {
        ui->tabWidget->setCurrentIndex(this->previousIndex);
        emit logoutTabDidCLick();
    }
}

void GeneralForm::tabBarClicked()
{
    this->previousIndex = ui->tabWidget->currentIndex();
}

void GeneralForm::setMessages(QList<MessageEntity> list)
{
    ui->messagesListTab->setMessageList(list);
}

void GeneralForm::setTopics(QList<TopicEntity> list)
{
    ui->topicsTab->setTopicsList(list);
}

int GeneralForm::getProgress()
{
    return ui->progressBar->value();
}

void GeneralForm::setProgress(int value)
{
    ui->progressBar->setValue(value);
}

int GeneralForm::getProgressMax()
{
    return ui->progressBar->maximum();
}

int GeneralForm::getProgressMin()
{
    return ui->progressBar->minimum();
}

QSize GeneralForm::getSize()
{
    return QSize(392, 533);
}

GeneralForm::~GeneralForm()
{
    delete ui;
}
