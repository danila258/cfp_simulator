#include "ObjectCreatorWidget.h"

ObjectCreatorWidget::ObjectCreatorWidget(QWidget* parent)
    : QWidget(parent), _objectRowWidgetsList(new QListWidget(this)), _list(new QComboBox(this))
{
    // create layouts
    auto* mainLayout = new QVBoxLayout(this);
    auto* firstRowButtons = new QHBoxLayout();
    auto* secondRowButtons = new QHBoxLayout();

    // add objects names to list
    for (auto object : gui::defaultObjects)
    {
        _list->addItem(object.className);
    }

    // create buttons
    auto* addButton = new QPushButton("Add", this);
    auto* removeButton = new QPushButton("Remove", this);

    auto* setDefaultButton = new QPushButton("Set default", this);
    auto* setRandomButton = new QPushButton("Set random", this);

    // connect buttons with slots
    connect(addButton, &QPushButton::clicked, this, &ObjectCreatorWidget::addButtonClicked);
    connect(removeButton, &QPushButton::clicked, this, &ObjectCreatorWidget::removeButtonClicked);

    connect(setDefaultButton, &QPushButton::clicked, this, &ObjectCreatorWidget::setDefaultButtonClicked);
    connect(setRandomButton, &QPushButton::clicked, this, &ObjectCreatorWidget::setRandomButtonClicked);

    // add objects to layouts
    firstRowButtons->addWidget( _list.get() );
    firstRowButtons->addWidget(addButton);

    secondRowButtons->addWidget(removeButton);
    secondRowButtons->addWidget(setDefaultButton);
    secondRowButtons->addWidget(setRandomButton);

    mainLayout->addLayout(firstRowButtons);
    mainLayout->addLayout(secondRowButtons);
    mainLayout->addWidget( _objectRowWidgetsList.get() );
}

std::vector<objectContent> ObjectCreatorWidget::getObjects()
{
    std::vector<objectContent> objects;

    // get all objects from list
    for (size_t i = 0; i < _objectRowWidgetsList->count(); ++i)
    {
        ObjectRowWidget* widget = qobject_cast<ObjectRowWidget*>(_objectRowWidgetsList->itemWidget(_objectRowWidgetsList->item(i)));
        objects.emplace_back(widget->getUserInput());
    }

    return objects;
}

void ObjectCreatorWidget::updateCountSlot()
{
    _curIndex = 0;

    for (size_t i = 0; i < _objectRowWidgetsList->count(); ++i)
    {
        ObjectRowWidget* widget = qobject_cast<ObjectRowWidget*>(_objectRowWidgetsList->itemWidget(_objectRowWidgetsList->item(i)));
        widget->setId(_curIndex);
        _curIndex += widget->getCount();
    }
}

void ObjectCreatorWidget::addButtonClicked()
{
    auto* item = new QListWidgetItem( _objectRowWidgetsList.get() );
    item->setSizeHint(QSize(100, 100));
    item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);

    size_t arrIndex = _list->currentIndex();

    // connect rowWidget with this*
    auto* rowWidget = new ObjectRowWidget(gui::defaultObjects[arrIndex], _curIndex, this);
    connect(rowWidget, SIGNAL(updateCountSignal()), SLOT(updateCountSlot()));

    _objectRowWidgetsList->setItemWidget(item, rowWidget);
    ++_curIndex;
}

void ObjectCreatorWidget::removeButtonClicked()
{
    for (auto item : _objectRowWidgetsList->selectedItems())
    {
        _curIndex -= getRow(item)->getCount();

        _objectRowWidgetsList->removeItemWidget(item);
        delete _objectRowWidgetsList->takeItem(_objectRowWidgetsList->row(item));
    }

    updateCountSlot();
}

void ObjectCreatorWidget::setDefaultButtonClicked()
{
    for (auto item : _objectRowWidgetsList->selectedItems())
    {
        std::vector<int> values;

        for (auto& object : findDefault(getRow(item)->getClassName()).fields)
        {
            values.emplace_back( object.val->toInt() );
        }

        getRow(item)->setValues(values);
    }
}

void ObjectCreatorWidget::setRandomButtonClicked()
{
    for (auto item : _objectRowWidgetsList->selectedItems())
    {
        std::random_device seed;
        std::default_random_engine generator(seed());
        std::uniform_int_distribution<int> distribution(8, 100);
        auto dice = std::bind(distribution, generator);

        std::vector<int> values;

        for (size_t i = 0; i < findDefault(getRow(item)->getClassName()).fields.size(); ++i)
        {
            values.emplace_back( dice() );
        }

        getRow(item)->setValues(values);
    }
}

ObjectRowWidget* ObjectCreatorWidget::getRow(QListWidgetItem* item)
{
    return qobject_cast<ObjectRowWidget*>(_objectRowWidgetsList->itemWidget(item));
}

defaultObject ObjectCreatorWidget::findDefault(const UniversalString& name)
{
    auto it = std::find_if(gui::defaultObjects.begin(), gui::defaultObjects.end(), [&name](const defaultObject& object) {
        return object.className == name;
    });

    if (it != gui::defaultObjects.end())
    {
        return *it;
    }

    throw std::runtime_error("default object not found");
}