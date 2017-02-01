#ifndef CELLTOPICITEM_H
#define CELLTOPICITEM_H

#include <listwidget.h>


namespace Ui {
class CellTopicItem;
}

/**
 * @brief The CellTopicItem class
 */

class CellTopicItem : public QWidget
{
    Q_OBJECT

public:
    explicit CellTopicItem(QWidget *parent = 0);
    ~CellTopicItem();

    static CellTopicItem *createCellWith(QString text, int qos, ListWidget *widget);

    QString getText();
    void setText(QString text);

    int getValue();
    void setValue(int number);

signals:
    void deleteButtonClickOn(int row);

private slots:
    void on_closeButton_clicked();

private:
    Ui::CellTopicItem *ui;
};

#endif // CELLTOPICITEM_H
