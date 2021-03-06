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

#ifndef CELLWITHEDITLINE_H
#define CELLWITHEDITLINE_H

#include <listwidget.h>
#include "customtextedit.h"

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

    static CellWithEditLine *createCellWith(QString imagePath, QString text, QString placeholder, ListWidget *widget, bool isMultiline);

    void setNumbersValidator();

    QImage getImage();
    void setImage(QString path);

    QString getText();
    void setText(QString text);

    QString getPlaceholderText();
    void setPlaceholderText(QString text);

    QString getInputText();
    void setInputText(QString text);

    void setPasswordMode(bool mode);

    void setLineEditClickFilter(bool flag);

    void setIsMultiline(bool isMultiline);

    void clear();

    CustomTextEdit *getLineEdit();

private:
    Ui::CellWithEditLine *ui;
    bool eventFilter(QObject *obj, QEvent *event);
    bool isMultiline;

signals:
    void didClick(QLineEdit *);
};

#endif // CELLWITHEDITLINE_H
