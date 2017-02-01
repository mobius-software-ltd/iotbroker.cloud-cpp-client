#ifndef ACCOUNTSLISTDIALOG_H
#define ACCOUNTSLISTDIALOG_H

#include <QDialog>
#include <cells/cellaccountitem.h>
#include <database/entities/accountentity.h>

namespace Ui {
class AccountsListDialog;
}

/**
 * @brief The AccountsListDialog class
 */

class AccountsListDialog : public QDialog
{
    Q_OBJECT

private:

    Ui::AccountsListDialog *ui;

    DQList<AccountEntity> accounts;

    void addCell(QString name, QString clientID, QString host, int port);

public:
    explicit AccountsListDialog(QWidget *parent = 0);
    ~AccountsListDialog();

    void setAccountList(DQList<AccountEntity> list);

signals:
    void deleteAccount(AccountEntity *account);
    void accountDidSelect(AccountEntity *account);
    void newAccountDidClick();

private slots:
    void rowDidSelect(QListWidgetItem* item);
    void addButtonDidClick();
    void deleteItemAt(int row);
};

#endif // ACCOUNTSLISTDIALOG_H
