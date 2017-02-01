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
