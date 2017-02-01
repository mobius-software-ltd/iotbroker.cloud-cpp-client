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

QString CellTopicItem::getText()
{
    return ui->label->text();
}

void CellTopicItem::setText(QString text)
{
    ui->label->setText(text);
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
