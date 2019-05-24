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

#include "sendmessagetab.h"
#include "ui_sendmessagetab.h"
#include <QMessageBox>
#include <QInputDialog>

static int const contentIndex = 0;
static int const topicIndex = 1;
static int const qosIndex = 2;
static int const retainIndex = 3;
static int const duplicateIndex = 4;

SendMessageTab::SendMessageTab(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SendMessageTab)
{
    ui->setupUi(this);

    this->setStyleSheet("QDialog {background-image: url(:/resources/resources/iot_broker_background.jpg) }");

    connect(ui->sendPushButton, SIGNAL(clicked()), this, SLOT(sendButtonDidClick()));


    this->contentCell = CellWithEditLine::createCellWith(":/resources/resources/settings.png", "Content", "content", ui->sendMessageListWidget, true);
    connect(this->contentCell->getLineEdit(), SIGNAL(focussed(bool)), this, SLOT(showContentMultilineWindow(bool)));
    this->topicCell = CellWithEditLine::createCellWith(":/resources/resources/settings.png", "Topic", "topic", ui->sendMessageListWidget, false);

}

void SendMessageTab::setQoSForSendMessagesTab(QList<QString> qosList) {
    this->qosCell = CellWithComboBox::createCellWith(":/resources/resources/settings.png", "QoS", qosList, "0", ui->sendMessageListWidget);
    if(qosList.size()>2) {
        this->retainCell = CellWithCheckbox::createCellWith(":/resources/resources/settings.png", "Retain", false, ui->sendMessageListWidget);
        this->dupCell = CellWithCheckbox::createCellWith(":/resources/resources/settings.png", "Duplicate", false, ui->sendMessageListWidget);
    }
}

void SendMessageTab::sendButtonDidClick()
{
    QList<QString> list = this->getInformation();

    if (this->isFieldsFill(list) == false) {
        QMessageBox *messageBox = new QMessageBox("Warning", "Please fill all fields", QMessageBox::Warning, QMessageBox::Ok, QMessageBox::Cancel, QMessageBox::NoButton, this);
        messageBox->exec();
    } else {
        MessageEntity message = MessageEntity();

        message.topicName = list.at(topicIndex);
        message.qos = list.at(qosIndex).toInt();
        message.content = list.at(contentIndex).toUtf8();
        if(list.size()>3) {
            message.isRetain = list.at(retainIndex).toInt();
            message.isDub = list.at(duplicateIndex).toInt();
        }
        message.incoming = false;

        emit donePublishForSending(message);

        this->contentCell->clear();
        this->topicCell->clear();
        this->qosCell->reset();
        if(list.size()>3) {
            this->retainCell->reset();
            this->dupCell->reset();
        }
    }
}

QList<QString> SendMessageTab::getInformation()
{
    QList<QString> list = QList<QString>();

    for (int i = 0; i < 5; i++) {
        QListWidgetItem *item = ui->sendMessageListWidget->item(i);
        if (item != NULL) {
            QWidget *widget = ui->sendMessageListWidget->itemWidget(item);
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

bool SendMessageTab::isFieldsFill(QList<QString> list) {

    bool flag = true;

    for (int i = 0; i < list.size(); i++) {
        if (list.at(i).isEmpty()) {
            flag = false;
        }
    }
    return flag;
}

SendMessageTab::~SendMessageTab()
{
    delete ui;
}

void SendMessageTab::showContentMultilineWindow(bool value) {

    if(value) {
        bool bOk;
        QString str = QInputDialog::getMultiLineText(this, "Enter Will content", "", this->contentCell->getInputText(), &bOk);
        this->contentCell->getLineEdit()->clearFocus();
        if (bOk)
        {
            this->contentCell->setInputText(str);
        }
   }
}
