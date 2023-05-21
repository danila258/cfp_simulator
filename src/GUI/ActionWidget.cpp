#include "ActionWidget.h"

ActionWidget::ActionWidget(const std::vector<actionContent>& actions, QWidget* parent)
    : QWidget(parent), _actionList(new QListWidget())
{
    auto* mainLayout = new QVBoxLayout();

    // remove button
    auto* removeButton = new QPushButton("Remove");
    connect(removeButton, SIGNAL(clicked()), this, SLOT(removeSlot()));

    for (const auto& action : actions)
    {
        auto* item = getItem();
        _actionList->addItem(item);
        _actionList->setItemWidget(item, new ObjectRowWidget(action, this));
    }

    // setting main layout
    mainLayout->addWidget(removeButton);
    mainLayout->addWidget( _actionList.get() );

    this->setLayout(mainLayout);
}

void ActionWidget::setActions(const std::vector<actionContent>& actions)
{
    _actionList->clear();

    for (const auto& action : actions)
    {
        auto* item = getItem();
        _actionList->addItem(item);
        _actionList->setItemWidget(item, new ObjectRowWidget(action, this));
    }
}

void ActionWidget::removeSlot(int index)
{

}

QListWidgetItem* ActionWidget::getItem()
{
    auto* item = new QListWidgetItem( _actionList.get() );
    item->setSizeHint(QSize(100, 100));
    item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);

    return item;
}
