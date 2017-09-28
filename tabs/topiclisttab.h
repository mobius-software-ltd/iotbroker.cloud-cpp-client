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

#ifndef TOPICLISTTAB_H
#define TOPICLISTTAB_H

#include <QWidget>
#include <cells/celltopicitem.h>
#include <cells/cellwitheditline.h>
#include <cells/cellwithcombobox.h>
#include <database/entities/topicentity.h>

namespace Ui {
class TopicListTab;
}

/**
 * @brief The TopicListTab class
 */

class TopicListTab : public QWidget
{
    Q_OBJECT

private:

    Ui::TopicListTab *ui;

    QList<TopicEntity> topics;

    CellWithEditLine *topicCell;
    CellWithComboBox *qosCell;

    void addCell(QString name, int qos);

public:
    explicit TopicListTab(QWidget *parent = 0);
    ~TopicListTab();

    void setTopicsList(QList<TopicEntity> list);

signals:
    void addNewTopic(TopicEntity topic);
    void deleteTopic(TopicEntity topic);

private slots:
    void addButtonDidClick();
    void deleteItemAt(int row);
};

#endif // TOPICLISTTAB_H
