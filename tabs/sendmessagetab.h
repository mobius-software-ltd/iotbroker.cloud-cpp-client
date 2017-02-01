#ifndef SENDMESSAGETAB_H
#define SENDMESSAGETAB_H

#include <QWidget>
#include <cells/cellwithcheckbox.h>
#include <cells/cellwithcombobox.h>
#include <cells/cellwitheditline.h>
#include <parser/messages/publish.h>

namespace Ui {
class SendMessageTab;
}

/**
 * @brief The SendMessageTab class
 */

class SendMessageTab : public QWidget
{
    Q_OBJECT

private:

    Ui::SendMessageTab *ui;

    QList<QString> getInformation();
    bool isFieldsFill(QList<QString> list);

public:
    explicit SendMessageTab(QWidget *parent = 0);
    ~SendMessageTab();

signals:
    void donePublishForSending(Publish *publish);

private slots:
    void sendButtonDidClick();
};

#endif // SENDMESSAGETAB_H
