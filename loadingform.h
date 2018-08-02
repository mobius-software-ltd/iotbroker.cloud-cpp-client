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

#ifndef LOADINGFORM_H
#define LOADINGFORM_H

#include <QWidget>
#include <QTimer>

namespace Ui {
class LoadingForm;
}

/**
 * @brief The LoadingForm class
 */

class LoadingForm : public QWidget
{
    Q_OBJECT

    QTimer *timer;

public:
    explicit LoadingForm(QWidget *parent = 0);
    ~LoadingForm();

    QSize getSize();

private:
    Ui::LoadingForm *ui;

private slots:
    void timeoutSlot();

signals:
    void loadingDidFinish();
};

#endif // LOADINGFORM_H