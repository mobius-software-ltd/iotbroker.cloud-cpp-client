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

#include "cellwithcombobox.h"
#include "ui_cellwithcombobox.h"
#include <QGraphicsPixmapItem>

CellWithComboBox::CellWithComboBox(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CellWithComboBox)
{
    ui->setupUi(this);
}

CellWithComboBox *CellWithComboBox::createCellWith(QString imagePath, QString text, QList<QString> values, QString currentValue, ListWidget *widget)
{
    CellWithComboBox *widgetForm = new CellWithComboBox();
    widgetForm->setImage(imagePath);
    widgetForm->setText(text);
    widgetForm->setValue(currentValue);
    widgetForm->setValues(values);
    QListWidgetItem *item = new QListWidgetItem();
    item->setSizeHint(QSize(0,38));
    widget->addItem(item);
    widget->setItemWidget(item, widgetForm);
    return widgetForm;
}

QImage CellWithComboBox::getImage()
{
    QGraphicsPixmapItem *item = (QGraphicsPixmapItem *)ui->graphicsView->items().at(0);
    QPixmap pixmap = item->pixmap();
    return pixmap.toImage();
}

void CellWithComboBox::setImage(QString path)
{
    QPixmap pixmap;
    pixmap.load(path);

    QGraphicsScene* scene = new QGraphicsScene();

    ui->graphicsView->setScene(scene);

    QGraphicsPixmapItem* item = new QGraphicsPixmapItem(pixmap);
    scene->addItem(item);
    ui->graphicsView->fitInView(scene->itemsBoundingRect() ,Qt::KeepAspectRatio);

    ui->graphicsView->show();
}

QString CellWithComboBox::getText()
{
    return ui->textLabel->text();
}

void CellWithComboBox::setText(QString text)
{
    ui->textLabel->setText(text);
}

QString CellWithComboBox::getValue()
{
    return ui->comboBox->currentText();
}

void CellWithComboBox::setValue(QString value)
{
    ui->comboBox->setCurrentText(value);
}

QList<QString> CellWithComboBox::getValues()
{
    QList<QString> values = QList<QString>();
    for (int i = 0; i < ui->comboBox->count(); i++) {
        values.append(ui->comboBox->itemText(i));
    }
    return values;
}

void CellWithComboBox::setValues(QList<QString> values)
{
    foreach (QString item, values) {
        ui->comboBox->addItem(item);
    }
}

CellWithComboBox::~CellWithComboBox()
{
    delete ui;
}
