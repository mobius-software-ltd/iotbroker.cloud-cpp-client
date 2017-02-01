#ifndef CELLMESSAGEITEM_H
#define CELLMESSAGEITEM_H

#include <listwidget.h>

namespace Ui {
class CellMessageItem;
}

/**
 * @brief The CellMessageItem class
 */

class CellMessageItem : public QWidget
{
    Q_OBJECT

public:
    explicit CellMessageItem(QWidget *parent = 0);
    ~CellMessageItem();

    static void createCellWith(QString title, QString text, bool isIncoming, int qos, ListWidget *widget);

    QString getTitle();
    void setTitle(QString title);

    QString getText();
    void setText(QString text);

    bool isIncoming();
    void setIncoming(bool isIncoming);

    int getQoS();
    void setQoS(int qos);

private:
    Ui::CellMessageItem *ui;
};

#endif // CELLMESSAGEITEM_H
