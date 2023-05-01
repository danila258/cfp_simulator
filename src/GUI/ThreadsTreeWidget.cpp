#include "ThreadsTreeWidget.h"

ThreadsTreeWidget::ThreadsTreeWidget(const std::vector<std::vector<objectContent>>& threads, QWidget* parent) : QWidget(parent)
{
    // create layouts
    auto* mainLayout = new QVBoxLayout();
    auto* buttonsLayout = new QHBoxLayout();

    // create buttons
    auto* addButton = new QPushButton("Add thread");
    buttonsLayout->addWidget(addButton);

    auto* removeButton = new QPushButton("Remove thread");
    buttonsLayout->addWidget(removeButton);

    // create tree widget
    _treeWidget.reset(new QTreeWidget(this));

    _treeWidget->setColumnCount(1);
    _treeWidget->setHeaderLabels(QStringList(QString("Threads")));

    QList<QTreeWidgetItem*> items;

    for (size_t i = 0; i < threads.size(); ++i)
    {
        items.append(new QTreeWidgetItem(static_cast<QTreeWidget *>(nullptr), QStringList(QString("Thread: %1").arg(i))));

        for (const auto& object : threads[i])
        {
            items.back()->addChild(new QTreeWidgetItem(items.back(), QStringList(QString("%1").arg(object.varName))));
        }
    }

    _treeWidget->insertTopLevelItems(0, items);

    // set main layout
    mainLayout->addLayout(buttonsLayout);
    mainLayout->addWidget( _treeWidget.get() );

    setLayout(mainLayout);
}