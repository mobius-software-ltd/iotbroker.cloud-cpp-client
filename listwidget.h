#ifndef LISTWIDGET_H
#define LISTWIDGET_H

#include <QListWidget>
#include <QDebug>

/**
 * @brief The ListWidget class
 */

class ListWidget : public QListWidget
{
    Q_OBJECT
public:
    explicit ListWidget(QWidget *parent = 0);

signals:

public slots:
};

#endif // LISTWIDGET_H
