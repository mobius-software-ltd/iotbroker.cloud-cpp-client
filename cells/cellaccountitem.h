#ifndef CELLACCOUNTITEM_H
#define CELLACCOUNTITEM_H

#include <listwidget.h>

namespace Ui {
class CellAccountItem;
}

/**
 * @brief The CellAccountItem class
 */

class CellAccountItem : public QWidget
{
    Q_OBJECT

public:
    explicit CellAccountItem(QWidget *parent = 0);
    ~CellAccountItem();

    static CellAccountItem *createCellWith(QString username, QString clientID, QString host, QString port, ListWidget *widget);

    QString getUsername();
    void setUsername(QString username);

    QString getClientID();
    void setClientID(QString clientID);

    QString getHost();
    void setHost(QString host);

    QString getPort();
    void setPort(QString port);

signals:
    void deleteButtonClickOn(int row);

private slots:
    void on_closeButton_clicked();

private:
    Ui::CellAccountItem *ui;
};

#endif // CELLACCOUNTITEM_H
