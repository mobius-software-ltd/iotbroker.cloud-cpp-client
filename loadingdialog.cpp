/**
 * Mobius Software LTD
 * Copyright 2015-2017, Mobius Software LTD
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

#include "loadingdialog.h"
#include "ui_loadingdialog.h"

LoadingDialog::LoadingDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoadingDialog)
{
    ui->setupUi(this);
    this->setStyleSheet("QDialog {background-image: url(:/resources/resources/iot_broker_background.jpg) }");

    ui->progressBar->setValue(0);

    this->timer = new QTimer(this);

    QObject::connect(timer, SIGNAL(timeout()), SLOT(timeoutSlot()));
    this->timer->setInterval(32);
    this->timer->start();
}

void LoadingDialog::timeoutSlot()
{
    if (ui->progressBar->value() < ui->progressBar->maximum() && ui->progressBar->value() >= ui->progressBar->minimum()) {
        ui->progressBar->setValue(ui->progressBar->value() + 1);
    } else {
        this->timer->stop();
        accept();
    }
}

LoadingDialog::~LoadingDialog()
{
    delete ui;
}
