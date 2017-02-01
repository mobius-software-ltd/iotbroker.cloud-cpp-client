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
    void addNewTopic(QString topicName, int qos);
    void deleteTopic(QString topicName, int qos);

private slots:
    void addButtonDidClick();
    void deleteItemAt(int row);
};

#endif // TOPICLISTTAB_H
