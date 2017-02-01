#ifndef CELLWITHCHECKBOX_H
#define CELLWITHCHECKBOX_H

#include <listwidget.h>

namespace Ui {
class CellWithCheckbox;
}

/**
 * @brief The CellWithCheckbox class
 */

class CellWithCheckbox : public QWidget
{
    Q_OBJECT

public:
    explicit CellWithCheckbox(QWidget *parent = 0);
    ~CellWithCheckbox();

    static CellWithCheckbox *createCellWith(QString imagePath, QString text, bool isChecked, ListWidget *widget);

    QImage getImage();
    void setImage(QString path);

    QString getText();
    void setText(QString text);

    bool getState();
    void setState(bool state);

private:
    Ui::CellWithCheckbox *ui;
};

#endif // CELLWITHCHECKBOX_H
