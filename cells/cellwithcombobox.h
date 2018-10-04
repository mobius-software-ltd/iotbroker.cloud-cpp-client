/**
 * Mobius Software LTD
 * Copyright 2015-2018, Mobius Software LTD
 *
 * This is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation; either version 2.1 of
 * the License, or (at your option) any later version.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this software; if not, write to the Free
 * Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301 USA, or see the FSF site: http://www.fsf.org.
 */

#ifndef CELLWITHCOMBOBOX_H
#define CELLWITHCOMBOBOX_H

#include <listwidget.h>
#include <QComboBox>

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

    static CellWithComboBox *createCellWith(QString imagePath, QString text, QList<QString> values, QString currentValue, ListWidget *widget);

    QImage getImage();
    void setImage(QString path);

    QString getText();
    void setText(QString text);

    QString getValue();
    void setValue(QString value);

    QList<QString> getValues();
    void setValues(QList<QString> values);

    QComboBox *getComboBox();

private:
    Ui::CellWithComboBox *ui;
};

#endif // CELLWITHCOMBOBOX_H
