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
