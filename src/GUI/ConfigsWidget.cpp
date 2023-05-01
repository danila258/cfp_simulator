#include "ConfigsWidget.h"

ConfigsWidget::ConfigsWidget(QWidget* parent) : QWidget(parent)
{
    // create layouts
    auto* mainLayout = new QVBoxLayout(this);
    auto* buttonsLayout = new QHBoxLayout();

    auto* list = new QListWidget();

    QVector<QString> names = {"Config1", "Config2", "Config3"};

    // add objects names to list
    for (auto config : names)
    {
        auto* item = new QListWidgetItem( config );
        item->setSizeHint(QSize(50, 50));
        item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable);

        list->addItem(item);
    }

    // create buttons
    auto* addButton = new QPushButton("Add config", this);
    auto* removeButton = new QPushButton("Remove config", this);

//    // connect buttons with slots
//    connect(addButton, &QPushButton::clicked, this, &ObjectCreatorWidget::addButtonClicked);
//    connect(removeButton, &QPushButton::clicked, this, &ObjectCreatorWidget::removeButtonClicked);

    // add objects to layouts
    buttonsLayout->addWidget(addButton);
    buttonsLayout->addWidget(removeButton);
    mainLayout->addLayout(buttonsLayout);

    mainLayout->addWidget(list);

    this->setLayout(mainLayout);
}