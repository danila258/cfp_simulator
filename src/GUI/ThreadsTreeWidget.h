#ifndef THREADTREEWIDGET_H
#define THREADTREEWIDGET_H

#include "UniversalString.h"
#include "DataTransferInterface.h"

#include <QWidget>
#include <QTreeWidget>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>


class ThreadsTreeWidget : public QWidget
{
Q_OBJECT

public:
    ThreadsTreeWidget(const std::vector<std::vector<objectContent>>& threads, QWidget* parent = nullptr);

private:
    std::shared_ptr<QTreeWidget> _treeWidget;
};


#endif //THREADTREEWIDGET_H
