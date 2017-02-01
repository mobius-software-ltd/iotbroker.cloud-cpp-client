#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include <cells/cellwitheditline.h>
#include <cells/cellwithcheckbox.h>
#include <cells/cellwithcombobox.h>
#include <database/entities/accountentity.h>

namespace Ui {
class LogInDialog;
}

/**
 * @brief The LogInDialog class
 */

class LogInDialog : public QDialog
{
    Q_OBJECT

private:

    Ui::LogInDialog *ui;

    CellWithEditLine *usernameCell;
    CellWithEditLine *passwordCell;
    CellWithEditLine *clientIDCell;
    CellWithEditLine *serverHostCell;
    CellWithEditLine *portCell;

    CellWithCheckbox *cleanSessionCell;
    CellWithEditLine *keepaliveCell;
    CellWithEditLine *willCell;
    CellWithEditLine *willTopicCell;
    CellWithCheckbox *retainCell;
    CellWithComboBox *qosCell;

    QList<QString> getInformation();
    bool isFieldsFill(QList<QString> list);

public:
    explicit LogInDialog(QWidget *parent = 0);
    ~LogInDialog();

signals:
    void accountToSave(AccountEntity account);

public slots:
    void logInButtonDidClick();
};

#endif // LOGINDIALOG_H
