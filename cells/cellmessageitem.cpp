/**
 * Mobius Software LTD
 * Copyright 2015-2017, Mobius Software LTD
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

#include "cellmessageitem.h"
#include "ui_cellmessageitem.h"

CellMessageItem::CellMessageItem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CellMessageItem)
{
    ui->setupUi(this);
}

void CellMessageItem::createCellWith(QString title, QString text, bool isIncoming, int qos, ListWidget *widget)
{
    CellMessageItem *widgetForm = new CellMessageItem();
    widgetForm->setTitle(title);
    widgetForm->setText(text);
    widgetForm->setIncoming(isIncoming);
    widgetForm->setQoS(qos);
    QListWidgetItem *item = new QListWidgetItem();

    QFont myFont("Halvetica", 13);

    QFontMetrics fm(myFont);
    double width=fm.width(text);

    double lineWidth = width / 300.0 + 1;

    item->setSizeHint(QSize(0, 50 + (12 * lineWidth)));
    widget->addItem(item);
    widget->setItemWidget(item, widgetForm);
}

QString CellMessageItem::getTitle()
{
    return ui->title->text();
}

void CellMessageItem::setTitle(QString title)
{
    ui->title->setText(title);
}

QString CellMessageItem::getText()
{
    return ui->text->text();
}

void CellMessageItem::setText(QString text)
{
    ui->text->setText(text);
}

bool CellMessageItem::isIncoming()
{
    return (ui->incomingLabel->text() == QString("in")) ? true : false;
}

void CellMessageItem::setIncoming(bool isIncoming)
{
    if (isIncoming == true) {
        ui->incomingLabel->setText("in");
    } else {
        ui->incomingLabel->setText("out");
    }
}

int CellMessageItem::getQoS()
{
    QList<QString> list = ui->qosLabel->text().split(":");
    if (list.size() == 2) {
        return list.at(1).toInt();
    }
    return -1;
}

void CellMessageItem::setQoS(int qos)
{
    ui->qosLabel->setText("QOS:"+QString::number(qos));
}

CellMessageItem::~CellMessageItem()
{
    delete ui;
}
