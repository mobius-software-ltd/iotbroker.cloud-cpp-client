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

#include "celltopicitem.h"
#include "ui_celltopicitem.h"

CellTopicItem::CellTopicItem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CellTopicItem)
{
    ui->setupUi(this);
}

CellTopicItem *CellTopicItem::createCellWith(QString text, int qos, ListWidget *widget)
{
    CellTopicItem *widgetForm = new CellTopicItem();
    widgetForm->setText(text);
    widgetForm->setValue(qos);
    QListWidgetItem *item = new QListWidgetItem();
    item->setSizeHint(QSize(0,38));
    widget->addItem(item);
    widget->setItemWidget(item, widgetForm);
    widgetForm->setProperty("row", widget->row(item));
    return widgetForm;
}

static QString modifyString (QString title, int width ){
    QString *tmpTitle;
    tmpTitle = new QString(title);

    int c = tmpTitle->length();

    if( c > width)
    {
        for(int i = 1; i <= c/width; i++)
        {
            int n = i * width;
            tmpTitle->insert(n, " ");
        }
    }
    return *tmpTitle;
}

QString CellTopicItem::getText()
{
    return ui->label->text();
}

void CellTopicItem::setText(QString text)
{
    ui->label->setText(modifyString(text, 30));
}

int CellTopicItem::getValue()
{
    QList<QString> list = ui->qosLabel->text().split(":");
    if (list.size() == 2) {
        return list.at(1).toInt();
    }
    return -1;
}

void CellTopicItem::setValue(int number)
{
    ui->qosLabel->setText("QOS:"+QString::number(number));
}

void CellTopicItem::on_closeButton_clicked()
{
    int row = this->property("row").toInt();
    emit deleteButtonClickOn(row);
}

CellTopicItem::~CellTopicItem()
{
    delete ui;
}
