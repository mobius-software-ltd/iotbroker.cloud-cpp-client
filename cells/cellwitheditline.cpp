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

#include "cellwitheditline.h"
#include "ui_cellwitheditline.h"
#include <QGraphicsPixmapItem>

CellWithEditLine::CellWithEditLine(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CellWithEditLine)
{
    ui->setupUi(this);
}

CellWithEditLine *CellWithEditLine::createCellWith(QString imagePath, QString text, QString placeholder, ListWidget *widget, bool isMultiline)
{

    CellWithEditLine *widgetForm = new CellWithEditLine();
    widgetForm->setImage(imagePath);
    widgetForm->setText(text);
    widgetForm->setPlaceholderText(placeholder);
    QListWidgetItem *item = new QListWidgetItem();
    item->setSizeHint(QSize(0,38));
    widget->addItem(item);
    widget->setItemWidget(item, widgetForm);
    return widgetForm;
}

void CellWithEditLine::setNumbersValidator()
{
    QRegExpValidator *validator = new QRegExpValidator(QRegExp("\\d*"), this);
    ui->lineEdit->setValidator(validator);
}

QImage CellWithEditLine::getImage()
{
    QGraphicsPixmapItem *item = (QGraphicsPixmapItem *)ui->graphicsView->items().at(0);
    QPixmap pixmap = item->pixmap();
    return pixmap.toImage();
}

void CellWithEditLine::setImage(QString path)
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

QString CellWithEditLine::getText()
{
    return ui->textLabel->text();
}

void CellWithEditLine::setText(QString text)
{
    ui->textLabel->setText(text);
}

QString CellWithEditLine::getPlaceholderText()
{
    return ui->lineEdit->placeholderText();
}

void CellWithEditLine::setPlaceholderText(QString text)
{
    ui->lineEdit->setPlaceholderText(text);
}

QString CellWithEditLine::getInputText()
{
    return ui->lineEdit->text();
}

void CellWithEditLine::setInputText(QString text)
{
    ui->lineEdit->setText(text);
}

void CellWithEditLine::setPasswordMode(bool mode) {
    if (mode) {
        ui->lineEdit->setEchoMode(QLineEdit::Password);
    } else {
        ui->lineEdit->setEchoMode(QLineEdit::Normal);
    }
}

void CellWithEditLine::setLineEditClickFilter(bool flag)
{
    if (flag) {
        ui->lineEdit->installEventFilter(this);
        ui->lineEdit->setReadOnly(true);
    } else {
        ui->lineEdit->removeEventFilter(this);
        ui->lineEdit->setReadOnly(false);
    }
}

void CellWithEditLine::clear()
{
    ui->lineEdit->clear();
}

CellWithEditLine::~CellWithEditLine()
{
    delete ui;
}

bool CellWithEditLine::eventFilter(QObject *obj, QEvent *event)
{
    Q_UNUSED(obj);
    if(!this->isMultiline && event->type() == QEvent::MouseButtonPress)
    {
        emit didClick(ui->lineEdit);
    }
    return false;
}

void CellWithEditLine::setIsMultiline(bool isMultiline)
{
    this->isMultiline = isMultiline;
}

CustomTextEdit *CellWithEditLine::getLineEdit()
{
    return ui->lineEdit;
}

