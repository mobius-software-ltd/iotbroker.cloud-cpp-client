#ifndef CELLWITHEDITLINE_H
#define CELLWITHEDITLINE_H

#include <listwidget.h>

namespace Ui {
class CellWithEditLine;
}

/**
 * @brief The CellWithEditLine class
 */

class CellWithEditLine : public QWidget
{
    Q_OBJECT

public:
    explicit CellWithEditLine(QWidget *parent = 0);
    ~CellWithEditLine();

    static CellWithEditLine *createCellWith(QString imagePath, QString text, QString placeholder, ListWidget *widget);

    QImage getImage();
    void setImage(QString path);

    QString getText();
    void setText(QString text);

    QString getPlaceholderText();
    void setPlaceholderText(QString text);

    QString getInputText();
    void setInputText(QString text);

private:
    Ui::CellWithEditLine *ui;
};

#endif // CELLWITHEDITLINE_H
