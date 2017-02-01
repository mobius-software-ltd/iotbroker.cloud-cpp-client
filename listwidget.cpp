#include "listwidget.h"
#include <QLabel>

ListWidget::ListWidget(QWidget *parent) : QListWidget(parent)
{
    QPalette palette = this->palette();
    palette.setColor(QPalette::Highlight, Qt::lightGray);
    this->setPalette(palette);
}
