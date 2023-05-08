#include "ConfigsWidget.h"

ConfigsWidget::ConfigsWidget(QWidget* parent) : QWidget(parent), _listWidget(new QListWidget(this))
{
    connect(_listWidget.get(), SIGNAL(currentItemChanged(QListWidgetItem*, QListWidgetItem*)),
            this, SLOT(changeConfigSlot(QListWidgetItem*, QListWidgetItem*)));

    // add first item
    _listWidget->addItem( getItem() );
    _listWidget->setCurrentItem( _listWidget->item(0) );

    // create layouts
    auto* mainLayout = new QVBoxLayout(this);
    auto* buttonsLayout = new QHBoxLayout();

    // create buttons
    auto* addButton = new QPushButton("Add config", this);
    connect(addButton, SIGNAL(clicked()), this, SLOT(addButtonClicked()));

    auto* removeButton = new QPushButton("Remove config", this);
    connect(removeButton, SIGNAL(clicked()), this, SLOT(removeButtonClicked()));

    // add objects to layouts
    buttonsLayout->addWidget(addButton);
    buttonsLayout->addWidget(removeButton);
    mainLayout->addLayout(buttonsLayout);

    mainLayout->addWidget( _listWidget.get() );

    this->setLayout(mainLayout);
}

std::vector<UniversalString> ConfigsWidget::getConfigNames()
{
    std::vector<UniversalString> names;

    for (int i = 0; i < _listWidget->count(); i++)
    {
        names.emplace_back( _listWidget->item(i)->text() );
    }

    return names;
}

QListWidgetItem* ConfigsWidget::getItem()
{
    auto* item = new QListWidgetItem("Config" + QString::number(_listWidget->count()));
    item->setSizeHint(QSize(50, 50));
    item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable);

    return item;
}

void ConfigsWidget::addButtonClicked()
{
    _listWidget->addItem( getItem() );
    emit addConfigSignal();
}

void ConfigsWidget::removeButtonClicked()
{
    for (auto item : _listWidget->selectedItems())
    {
        if (_listWidget->count() == 1)
        {
            break;
        }

        int index = _listWidget->row(item);
        delete _listWidget->takeItem(index);
        emit removeConfigSignal(index);
    }
}

void ConfigsWidget::changeConfigSlot(QListWidgetItem* current, QListWidgetItem* previous)
{
    emit changeConfigIndexSignal( _listWidget->row(current) );
}