#include "ThreadsTreeWidget.h"

ThreadsTreeWidget::ThreadsTreeWidget(const std::vector<std::vector<objectContent>>& threads, QWidget* parent) : QWidget(parent)
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

void ThreadsTreeWidget::setThreads(const std::vector<std::vector<objectContent>>& threads)
{
    QList<QTreeWidgetItem*> items;

    for (const auto & thread : threads)
    {
        items.append( getThreadItem() );

        for (const auto& object : thread)
        {
            items.back()->addChild( getChildItem(items.back(), object) );
        }
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

void ThreadsTreeWidget::updateCurrentThread(const std::vector<objectContent>& content)
{
    QTreeWidgetItem* item = _treeWidget->currentItem();

    // delete all children
    for (auto* child : item->takeChildren())
    {
        delete child;
    }

    // add new children
    for (const auto& object : content)
    {
        item->addChild(getChildItem(item, object));
    }
}

QTreeWidgetItem* ThreadsTreeWidget::getThreadItem()
{
    return new QTreeWidgetItem(static_cast<QTreeWidget*>(nullptr), QStringList(QString("Thread: %1").arg(_treeWidget->topLevelItemCount() + 1)));
}

QTreeWidgetItem* ThreadsTreeWidget::getChildItem(QTreeWidgetItem* parent, const objectContent& object)
{
    auto* item = new QTreeWidgetItem(parent, QStringList(QString("%1").arg(object.varName)));
    item->setFlags(item->flags() & ~Qt::ItemIsSelectable);
    return item;
}

void ThreadsTreeWidget::addButtonSlot()
{
    _treeWidget->addTopLevelItem( getThreadItem() );
    emit addThreadSignal();
}

void ThreadsTreeWidget::removeButtonSlot()
{
    QList<QTreeWidgetItem*> items = _treeWidget->selectedItems();

    for (auto* item : items)
    {
        if ( _treeWidget->topLevelItemCount() == 1)
        {
            break;
        }

        int index = _treeWidget->indexOfTopLevelItem(item);
        emit removeThreadSignal(index);
        delete _treeWidget->takeTopLevelItem(index);
    }
}

void ThreadsTreeWidget::changeThreadSlot(QTreeWidgetItem* current, QTreeWidgetItem* previous)
{
    // only root items have not parent
    if (current->parent() != nullptr)
    {
        changeThreadSignal( _treeWidget->indexOfTopLevelItem(current->parent()) );
        return;
    }

    changeThreadSignal(_treeWidget->indexOfTopLevelItem(current));
}