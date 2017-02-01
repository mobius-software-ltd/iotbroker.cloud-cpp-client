#ifndef CELLWITHCOMBOBOX_H
#define CELLWITHCOMBOBOX_H

#include <listwidget.h>

namespace Ui {
class CellWithComboBox;
}

/**
 * @brief The CellWithComboBox class
 */

class CellWithComboBox : public QWidget
{
    Q_OBJECT

public:
    explicit CellWithComboBox(QWidget *parent = 0);
    ~CellWithComboBox();

    static CellWithComboBox *createCellWith(QString imagePath, QString text, QString value, ListWidget *widget);

    QImage getImage();
    void setImage(QString path);

    QString getText();
    void setText(QString text);

    QString getValue();
    void setValue(QString value);

private:
    Ui::CellWithComboBox *ui;
};

#endif // CELLWITHCOMBOBOX_H
