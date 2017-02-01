#include "cellwitheditline.h"
#include "ui_cellwitheditline.h"
#include <QGraphicsPixmapItem>

CellWithEditLine::CellWithEditLine(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CellWithEditLine)
{
    ui->setupUi(this);
}

CellWithEditLine *CellWithEditLine::createCellWith(QString imagePath, QString text, QString placeholder, ListWidget *widget)
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

CellWithEditLine::~CellWithEditLine()
{
    delete ui;
}
