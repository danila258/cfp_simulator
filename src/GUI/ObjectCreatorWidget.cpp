#include "ObjectCreatorWidget.h"

ObjectCreatorWidget::ObjectCreatorWidget(QWidget* parent)
    : QWidget(parent), _objectRowWidgetsList(new QListWidget(this)), _list(new QComboBox(this))
{
    // create layouts
    auto* mainLayout = new QVBoxLayout(this);
    auto* firstRowButtons = new QHBoxLayout();
    auto* secondRowButtons = new QHBoxLayout();

    // add objects names to list
    for (const auto& object : gui::defaultObjects)
    {
        _list->addItem(object.className);
    }

    // create buttons
    auto* addButton = new QPushButton("Add", this);
    auto* removeButton = new QPushButton("Remove", this);

    auto* setDefaultButton = new QPushButton("Set default", this);
    auto* setRandomButton = new QPushButton("Set random", this);

    // connect buttons with slots
    connect(addButton, SIGNAL(clicked()), this, SLOT(addButtonSlot()));
    connect(addButton, SIGNAL(clicked()), this, SLOT(userInputSlot()));

    connect(removeButton, SIGNAL(clicked()), this, SLOT(removeButtonSlot()));
    connect(removeButton, SIGNAL(clicked()), this, SLOT(userInputSlot()));


    connect(setDefaultButton, SIGNAL(clicked()), this, SLOT(setDefaultButtonSlot()));
    connect(setDefaultButton, SIGNAL(clicked()), this, SLOT(userInputSlot()));

    connect(setRandomButton, SIGNAL(clicked()), this, SLOT(setRandomButtonSlot()));
    connect(setRandomButton, SIGNAL(clicked()), this, SLOT(userInputSlot()));

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

void ObjectCreatorWidget::setObjects(const std::vector<objectContent>& objects)
{
    _objectRowWidgetsList->clear();
    _curId = 0;

    for (const auto& object : objects)
    {
        _objectRowWidgetsList->setItemWidget(getItem(), new ObjectRowWidget(findDefault(object.className), _curId, object, this));
        _curId += object.count;
    }
}

std::vector<objectContent> ObjectCreatorWidget::getObjects()
{
    std::vector<objectContent> objects;

    // get all objects from list
    qDebug() << _objectRowWidgetsList->count();
    for (size_t i = 0; i < _objectRowWidgetsList->count(); ++i)
    {
        auto* widget = qobject_cast<ObjectRowWidget*>(_objectRowWidgetsList->itemWidget(_objectRowWidgetsList->item(i)));
        objects.emplace_back( widget->getUserInput() );
    }

    return objects;
}

void ObjectCreatorWidget::updateCountSlot()
{
    _curId = 0;

    for (size_t i = 0; i < _objectRowWidgetsList->count(); ++i)
    {
        auto* widget = qobject_cast<ObjectRowWidget*>(_objectRowWidgetsList->itemWidget(_objectRowWidgetsList->item(i)));
        widget->setId(_curId);
        _curId += widget->getCount();
    }
}

void ObjectCreatorWidget::addButtonSlot()
{
    size_t arrIndex = _list->currentIndex();

    // connect rowWidget with this*
    auto* rowWidget = new ObjectRowWidget(gui::defaultObjects[arrIndex], _curId, this);
    connect(rowWidget, SIGNAL(updateObjectSignal()), SLOT(userInputSlot()));
    connect(rowWidget, SIGNAL(updateCountSignal()), SLOT(updateCountSlot()));

    _objectRowWidgetsList->setItemWidget(getItem(), rowWidget);
    ++_curId;
}

void ObjectCreatorWidget::removeButtonSlot()
{
    for (auto item : _objectRowWidgetsList->selectedItems())
    {
        _curId -= getRow(item)->getCount();

        _objectRowWidgetsList->removeItemWidget(item);
        delete _objectRowWidgetsList->takeItem(_objectRowWidgetsList->row(item));
    }

    updateCountSlot();
}

void ObjectCreatorWidget::setDefaultButtonSlot()
{
    for (auto item : _objectRowWidgetsList->selectedItems())
    {
        std::vector<UniversalString> values;

        for (auto& object : findDefault(getRow(item)->getClassName()).fields)
        {
            values.emplace_back( object.val);
        }

        getRow(item)->setValues(values);
    }
}

void ObjectCreatorWidget::setRandomButtonSlot()
{
    for (auto item : _objectRowWidgetsList->selectedItems())
    {
        std::random_device seed;
        std::default_random_engine generator(seed());
        std::uniform_int_distribution<int> distribution(8, 100);
        auto dice = std::bind(distribution, generator);

        std::vector<UniversalString> values;

        for (size_t i = 0; i < findDefault(getRow(item)->getClassName()).fields.size(); ++i)
        {
            values.emplace_back( QString::number(dice()) );
        }

        getRow(item)->setValues(values);
    }
}

void ObjectCreatorWidget::userInputSlot()
{
    emit updateThreadSignal( getObjects() );
}

QListWidgetItem* ObjectCreatorWidget::getItem()
{
    auto* item = new QListWidgetItem( _objectRowWidgetsList.get() );
    item->setSizeHint(QSize(100, 100));
    item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);

    return item;
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