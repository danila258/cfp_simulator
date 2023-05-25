#include "ThreadsTreeWidget.h"

ThreadsTreeWidget::ThreadsTreeWidget(const std::vector<threadContent>& threads, QWidget* parent) : QWidget(parent)
{
    // create layouts
    auto* mainLayout = new QVBoxLayout();
    auto* buttonsLayout = new QHBoxLayout();

    // create buttons
    auto* addButton = new QPushButton("Add thread");
    connect(addButton, SIGNAL(clicked()), this, SLOT(addButtonSlot()));
    buttonsLayout->addWidget(addButton);

    auto* removeButton = new QPushButton("Remove thread");
    connect(removeButton, SIGNAL(clicked()), this, SLOT(removeButtonSlot()));
    buttonsLayout->addWidget(removeButton);

    // create tree widget
    _treeWidget.reset(new QTreeWidget(this));
    connect(_treeWidget.get(), SIGNAL(currentItemChanged(QTreeWidgetItem*, QTreeWidgetItem*)),
            this, SLOT(changeThreadSlot(QTreeWidgetItem*, QTreeWidgetItem*)));
    _treeWidget->setColumnCount(1);
    _treeWidget->setHeaderLabels(QStringList(QString("Threads")));

    setThreads(threads);

    // set main layout
    mainLayout->addLayout(buttonsLayout);
    mainLayout->addWidget( _treeWidget.get() );

    setLayout(mainLayout);
}

void ThreadsTreeWidget::setThreads(const std::vector<threadContent>& threads)
{
    _treeWidget->clear();
    QList<QTreeWidgetItem*> items;

    for (const auto & thread : threads)
    {
        items.append( getThreadItem(thread.number) );
        items.back()->addChildren( getThreadChildItems(items.back(), thread.objects) );
    }

    // add first thread
    if ( threads.empty() )
    {
        auto* item = getThreadItem();
        items.append( item );
    }

    _treeWidget->insertTopLevelItems(0, items);
    _treeWidget->setCurrentItem( _treeWidget->topLevelItem(0) );
}

void ThreadsTreeWidget::updateThreadTreeSlot(const std::vector<objectContent>& content)
{
    QTreeWidgetItem* item = _treeWidget->currentItem();

    // only root items haven't parent
    if (item->parent() != nullptr)
    {
        item = item->parent();
    }

    // delete all children
    for (auto* child : item->takeChildren())
    {
        delete child;
    }

    // add new children
    qDebug() << "update threads items";
    item->addChildren( getThreadChildItems(item, content) );
}

QTreeWidgetItem* ThreadsTreeWidget::getThreadItem()
{
    return getThreadItem( _treeWidget->topLevelItemCount() );
}

QTreeWidgetItem* ThreadsTreeWidget::getThreadItem(int n)
{
    return new QTreeWidgetItem( static_cast<QTreeWidget*>(nullptr),QStringList(getThreadName(n)) );
}

QString ThreadsTreeWidget::getThreadName(int n)
{
    return QString("Thread: %1").arg(n);
}

QList<QTreeWidgetItem*> ThreadsTreeWidget::getThreadChildItems(QTreeWidgetItem* parent, const std::vector<objectContent>& objects)
{
    std::unordered_map<QString, int> map;
    QList<QTreeWidgetItem*> items;

    for (auto& object : objects)
    {
        auto it = map.find(object.className);

        if (it == map.end())
        {
            map[object.className] = object.count;
        }
        else
        {
            it->second += object.count;
        }
    }

    for (auto& pair : map)
    {
        items.push_back(new QTreeWidgetItem(parent,
                                            QStringList(QString("%1 %2").arg(pair.first, QString::number(pair.second)))));
    }

    return items;
}

void ThreadsTreeWidget::addButtonSlot()
{
    _treeWidget->addTopLevelItem( getThreadItem() );
    emit addThreadSignal();
}

void ThreadsTreeWidget::removeButtonSlot()
{
    for (auto* item : _treeWidget->selectedItems())
    {
        // don't delete last thread
        if ( _treeWidget->topLevelItemCount() == 1)
        {
            break;
        }

        int index = _treeWidget->indexOfTopLevelItem(item);
        delete _treeWidget->takeTopLevelItem(index);
        emit removeThreadSignal(index);
    }

    for (int i = 0; i < _treeWidget->topLevelItemCount(); ++i)
    {
        _treeWidget->topLevelItem(i)->setText(0, getThreadName(i));
    }
}

void ThreadsTreeWidget::changeThreadSlot(QTreeWidgetItem* current, QTreeWidgetItem* previous)
{
    // when use clear in setThreads method
    if (current == nullptr)
    {
        return;
    }

    // only root items haven't parent
    if (current->parent() != nullptr)
    {
        changeThreadIndexSignal(_treeWidget->indexOfTopLevelItem(current->parent()));
        return;
    }

    changeThreadIndexSignal(_treeWidget->indexOfTopLevelItem(current));
}