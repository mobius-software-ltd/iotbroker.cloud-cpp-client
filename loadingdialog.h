#ifndef LOADINGDIALOG_H
#define LOADINGDIALOG_H

#include <QDialog>
#include <QTimer>

namespace Ui {
class LoadingDialog;
}

/**
 * @brief The LoadingDialog class
 */

class LoadingDialog : public QDialog
{
    Q_OBJECT

    QTimer *timer;

public:
    explicit LoadingDialog(QWidget *parent = 0);
    ~LoadingDialog();

private:
    Ui::LoadingDialog *ui;

private slots:
    void timeoutSlot();
};

#endif // LOADINGDIALOG_H
