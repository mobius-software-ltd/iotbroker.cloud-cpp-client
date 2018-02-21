#ifndef ACCOUNTLISTFORM_H
#define ACCOUNTLISTFORM_H

#include <QWidget>

namespace Ui {
class AccountListForm;
}

class AccountListForm : public QWidget
{
    Q_OBJECT

public:
    explicit AccountListForm(QWidget *parent = 0);
    ~AccountListForm();

private:
    Ui::AccountListForm *ui;
};

#endif // ACCOUNTLISTFORM_H
