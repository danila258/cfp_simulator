#ifndef THREADTREEWIDGET_H
#define THREADTREEWIDGET_H

#include "UniversalString.h"
#include "DataTransferInterface.h"

#include <QWidget>
#include <QTreeWidget>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDebug>

#include <unordered_map>


class ThreadsTreeWidget : public QWidget
{
Q_OBJECT

public:
    ThreadsTreeWidget(const std::vector<std::vector<objectContent>>& threads, QWidget* parent = nullptr);

    void setThreads(const std::vector<std::vector<objectContent>>& threads);

public slots:
    void updateThreadTreeSlot(const std::vector<objectContent>& content);

signals:
    void changeThreadIndexSignal(int);
    void addThreadSignal();
    void removeThreadSignal(int);

private:
    std::shared_ptr<QTreeWidget> _treeWidget;

    QTreeWidgetItem* getThreadItem();
    QList<QTreeWidgetItem*> getThreadChildItems(QTreeWidgetItem* parent, const std::vector<objectContent>& objects);

    QString getThreadName();
    QString getThreadName(int n);

private slots:
    void addButtonSlot();
    void removeButtonSlot();
    void changeThreadSlot(QTreeWidgetItem* current, QTreeWidgetItem* previous);
};


#endif //THREADTREEWIDGET_H
