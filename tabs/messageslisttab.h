#ifndef MESSAGESLISTTAB_H
#define MESSAGESLISTTAB_H

#include <QWidget>
#include <cells/cellmessageitem.h>
#include <database/entities/messageentity.h>

namespace Ui {
class MessagesListTab;
}

/**
 * @brief The MessagesListTab class
 */

class MessagesListTab : public QWidget
{
    Q_OBJECT

private:

    Ui::MessagesListTab *ui;

    QList<MessageEntity> messages;

    void addCell(QString topicName, QString content, bool isIncoming, int qos);

public:
    explicit MessagesListTab(QWidget *parent = 0);
    ~MessagesListTab();

    void setMessageList(QList<MessageEntity> list);
};

#endif // MESSAGESLISTTAB_H
