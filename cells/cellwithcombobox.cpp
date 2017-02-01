#include "cellwithcombobox.h"
#include "ui_cellwithcombobox.h"
#include <QGraphicsPixmapItem>

CellWithComboBox::CellWithComboBox(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CellWithComboBox)
{
    ui->setupUi(this);
}

CellWithComboBox *CellWithComboBox::createCellWith(QString imagePath, QString text, QString value, ListWidget *widget)
{
    CellWithComboBox *widgetForm = new CellWithComboBox();
    widgetForm->setImage(imagePath);
    widgetForm->setText(text);
    widgetForm->setValue(value);
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

CellWithComboBox::~CellWithComboBox()
{
    delete ui;
}
