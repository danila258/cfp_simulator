#include "ObjectCreatorWidget.h"

ObjectCreatorWidget::ObjectCreatorWidget(const QVector<QPair<QString, QVector<QPair<QString, QString>>>>& defaultObjects, QWidget* parent)
    : QWidget(parent), _defaultObjects(defaultObjects), _objectRowWidgetsList(new QListWidget(this)), _list(new QComboBox(this))
{
    // create layouts
    auto* mainLayout = new QVBoxLayout(this);
    auto* firstRowButtons = new QHBoxLayout();
    auto* secondRowButtons = new QHBoxLayout();

    // add objects names to list
    for (auto object : defaultObjects)
    {
        _list->addItem(object.first);
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
    firstRowButtons->addWidget(_list);
    firstRowButtons->addWidget(addButton);

    secondRowButtons->addWidget(removeButton);
    secondRowButtons->addWidget(setDefaultButton);
    secondRowButtons->addWidget(setRandomButton);

    mainLayout->addLayout(firstRowButtons);
    mainLayout->addLayout(secondRowButtons);
    mainLayout->addWidget(_objectRowWidgetsList);
}

ObjectCreatorWidget::~ObjectCreatorWidget()
{
    delete _objectRowWidgetsList;
    delete _list;
}

QVector<RTLibObject> ObjectCreatorWidget::getObjects()
{
    QVector<RTLibObject> objects;

    // get all objects from list
    for (auto object : _objectRowWidgetsList->children())
    {
        objects.push_back(qobject_cast<ObjectRowWidget*>(object)->getUserInput());
    }

    return objects;
}

void ObjectCreatorWidget::addButtonClicked()
{
    auto* item = new QListWidgetItem(_objectRowWidgetsList);
    item->setSizeHint(QSize(100, 100));
    item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);

    size_t arrIndex = _list->currentIndex();

    _objectRowWidgetsList->setItemWidget(item,
 new ObjectRowWidget({_curIndex, _defaultObjects[arrIndex].first + QString::number(_curIndex), _defaultObjects[arrIndex].first, _defaultObjects[arrIndex].second}, this));

    ++_curIndex;
}

void ObjectCreatorWidget::removeButtonClicked()
{
    for (auto item : _objectRowWidgetsList->selectedItems())
    {
        --_curIndex;

        _objectRowWidgetsList->removeItemWidget(item);
        delete _objectRowWidgetsList->takeItem(_objectRowWidgetsList->row(item));
    }

    for (int i = 0; i < _objectRowWidgetsList->count(); ++i)
    {
        qobject_cast<ObjectRowWidget*>(_objectRowWidgetsList->itemWidget(_objectRowWidgetsList->item(i)))->setId(i);
    }
}

void ObjectCreatorWidget::setDefaultButtonClicked()
{
    for (auto item : _objectRowWidgetsList->selectedItems())
    {
        QVector<QString> values;

        for (auto& object : _defaultObjects[_list->currentIndex()].second)
        {
            values.push_back(object.second);
        }

        qobject_cast<ObjectRowWidget*>(_objectRowWidgetsList->itemWidget(item))->setValues(values);
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

        QVector<QString> values;

        for (int i = 0; i < _defaultObjects[_list->currentIndex()].second.size(); ++i)
        {
            values.push_back( QString::number(dice()) );
        }

        qobject_cast<ObjectRowWidget*>(_objectRowWidgetsList->itemWidget(item))->setValues(values);
    }
}