#include "ObjectCreatorWidget.h"

ObjectCreatorWidget::ObjectCreatorWidget(size_t thread, QWidget* parent)
    : QWidget(parent), _objectRowWidgetList(new QListWidget(this)), _classNamelist(new QComboBox(this)), _thread(thread)
{
    // create layouts
    auto* mainLayout = new QVBoxLayout();
    auto* firstRowButtons = new QHBoxLayout();
    auto* secondRowButtons = new QHBoxLayout();

    // add objects names to list
    for (const auto& object : gui::defaultObjects)
    {
        _classNamelist->addItem(object.name);
    }

    // create buttons
    auto* addObjectButton = new QPushButton("Add object", this);
    auto* addActionButton = new QPushButton("Add action...", this);

    auto* removeButton = new QPushButton("Remove", this);
    auto* setDefaultButton = new QPushButton("Set default", this);
    auto* setRandomButton = new QPushButton("Set random", this);

    // connect buttons with slots
    connect(addObjectButton, SIGNAL(clicked()), this, SLOT(addObjectSlot()));
    connect(addObjectButton, SIGNAL(clicked()), this, SLOT(userInputSlot()));

    connect(addActionButton, SIGNAL(clicked()), this, SLOT(addActionSlot()));

    connect(removeButton, SIGNAL(clicked()), this, SLOT(removeObjectSlot()));
    connect(removeButton, SIGNAL(clicked()), this, SLOT(userInputSlot()));

    connect(setDefaultButton, SIGNAL(clicked()), this, SLOT(setDefaultSlot()));
    connect(setDefaultButton, SIGNAL(clicked()), this, SLOT(userInputSlot()));

    connect(setRandomButton, SIGNAL(clicked()), this, SLOT(setRandomSlot()));
    connect(setRandomButton, SIGNAL(clicked()), this, SLOT(userInputSlot()));

    // add objects to layouts
    firstRowButtons->addWidget(_classNamelist.get() );
    firstRowButtons->addWidget(addObjectButton);
    firstRowButtons->addWidget(addActionButton);

    secondRowButtons->addWidget(removeButton);
    secondRowButtons->addWidget(setDefaultButton);
    secondRowButtons->addWidget(setRandomButton);

    mainLayout->addLayout(firstRowButtons);
    mainLayout->addLayout(secondRowButtons);

    mainLayout->addWidget(_objectRowWidgetList.get() );
    this->setLayout(mainLayout);
}

void ObjectCreatorWidget::setObjects(size_t thread, const std::vector<objectContent>& objects)
{
    _objectRowWidgetList->clear();
    _thread = thread;
    _id = 0;

    for (const auto& object : objects)
    {
        _objectRowWidgetList->setItemWidget(getItem(), new ObjectRowWidget(findDefault(object.className), _id, object, this));
        _id += object.count;
    }
}

std::vector<objectContent> ObjectCreatorWidget::getObjects()
{
    std::vector<objectContent> objects;

    // get all objects from list
    for (int i = 0; i < _objectRowWidgetList->count(); ++i)
    {
        auto* widget = qobject_cast<ObjectRowWidget*>(_objectRowWidgetList->itemWidget(_objectRowWidgetList->item(i)));
        objects.emplace_back( widget->getUserInput() );
    }

    return objects;
}

void ObjectCreatorWidget::updateCountSlot()
{
    _id = 0;

    for (int i = 0; i < _objectRowWidgetList->count(); ++i)
    {
        auto* widget = qobject_cast<ObjectRowWidget*>(_objectRowWidgetList->itemWidget(_objectRowWidgetList->item(i)));
        widget->setId(_id);
        _id += widget->getCount();
    }
}

void ObjectCreatorWidget::addObjectSlot()
{
    size_t arrIndex = _classNamelist->currentIndex();

    // connect rowWidget with this*
    auto* rowWidget = new ObjectRowWidget(gui::defaultObjects[arrIndex], _id, this);
    connect(rowWidget, SIGNAL(updateObjectSignal()), SLOT(userInputSlot()));
    connect(rowWidget, SIGNAL(updateCountSignal()), SLOT(updateCountSlot()));

    _objectRowWidgetList->setItemWidget(getItem(), rowWidget);
    ++_id;
}

void ObjectCreatorWidget::addActionSlot()
{
    for (auto item : _objectRowWidgetList->selectedItems())
    {
        auto* row = getRow(item);
        auto className = row->getClassName();
        std::unique_ptr<ActionDialog> dialog( new ActionDialog(_thread, className, gui::actionMap.find(className)->second,
                                                               row->getId(), row->getId() + row->getCount() - 1, this) );
        dialog->show();
        auto result = dialog->exec();

        if (result == QDialog::Accepted)
        {
            emit addActionSignal( dialog->getAction() );
        }
    }
}

void ObjectCreatorWidget::removeObjectSlot()
{
    for (auto item : _objectRowWidgetList->selectedItems())
    {
        _id -= getRow(item)->getCount();

        _objectRowWidgetList->removeItemWidget(item);
        delete _objectRowWidgetList->takeItem(_objectRowWidgetList->row(item));
    }

    updateCountSlot();
}

void ObjectCreatorWidget::setDefaultSlot()
{
    for (auto item : _objectRowWidgetList->selectedItems())
    {
        std::vector<UniversalString> values;

        for (auto& object : findDefault(getRow(item)->getClassName()).fields)
        {
            values.emplace_back( object.val);
        }

        getRow(item)->setValues(values);
    }
}

void ObjectCreatorWidget::setRandomSlot()
{
    for (auto item : _objectRowWidgetList->selectedItems())
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
    auto* item = new QListWidgetItem(_objectRowWidgetList.get() );
    item->setSizeHint(QSize(100, 100));
    item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);

    return item;
}

ObjectRowWidget* ObjectCreatorWidget::getRow(QListWidgetItem* item)
{
    return qobject_cast<ObjectRowWidget*>(_objectRowWidgetList->itemWidget(item));
}

defaultParam ObjectCreatorWidget::findDefault(const UniversalString& name)
{
    auto it = std::find_if(gui::defaultObjects.begin(), gui::defaultObjects.end(), [&name](const defaultParam& object) {
        return object.name == name;
    });

    if (it != gui::defaultObjects.end())
    {
        return *it;
    }

    throw std::runtime_error("default object not found");
}