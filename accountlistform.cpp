#include "accountlistform.h"
#include "ui_accountlistform.h"

AccountListForm::AccountListForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AccountListForm)
{
    ui->setupUi(this);
}

AccountListForm::~AccountListForm()
{
    delete ui;
}
