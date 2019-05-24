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

#ifndef GENERALFORM_H
#define GENERALFORM_H

#include <QWidget>
#include "database/entities/topicentity.h"
#include "database/entities/messageentity.h"

namespace Ui {
class GeneralForm;
}

/**
 * @brief The GeneralForm class
 */

class GeneralForm : public QWidget
{
    Q_OBJECT

    int previousIndex;

public:
    explicit GeneralForm(QWidget *parent = 0);
    ~GeneralForm();

    void setMessages(QList<MessageEntity> list);
    void setTopics(QList<TopicEntity> list);
    void setQoSForMessagesAndTopicsTab(int protocol);

    int getProgress();
    void setProgress(int value);

    int getProgressMax();
    int getProgressMin();

    QSize getSize();

private:
    Ui::GeneralForm *ui;

private slots:
    void willSubscribeToTopicSlot(TopicEntity topicEntity);
    void willUnsubscribeFromTopicSlot(TopicEntity topicEntity);

    void willPublishSlot(MessageEntity message);

    void currentTabChangedSlot(int index);
    void tabBarClicked();

signals:
    void willSubscribeToTopic(TopicEntity topicEntity);
    void willUnsubscribeFromTopic(TopicEntity topicEntity);

    void willPublish(MessageEntity message);

    void topicsTabDidClick();
    void messagesTabDidCLick();
    void logoutTabDidCLick();
};

#endif // GENERALFORM_H
