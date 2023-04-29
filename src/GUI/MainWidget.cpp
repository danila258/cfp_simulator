#include "MainWidget.h"
#include "UniversalString.h"


MainWidget::MainWidget()
{
    ConfigParser config("/home/danila/cpp/cfp_simulator/src/config.json");
    auto r = config.getThreads();
    auto g = config.getActions();

    config.writeConfig(r, g);

    _objectCreatorWidget = new ObjectCreatorWidget(this);

    // create layouts
    auto* mainLayout = new QVBoxLayout(this);
    auto* buttonsLayout = new QHBoxLayout();

    // create buttons
    auto* openButton = new QPushButton("Open", this);
    auto* saveButton = new QPushButton("Save", this);
    auto* runButton = new QPushButton("Run", this);

    // add widgets to layouts
    mainLayout->addWidget(_objectCreatorWidget);

    buttonsLayout->addWidget(openButton);
    buttonsLayout->addWidget(saveButton);
    buttonsLayout->addWidget(runButton);

    // config main layout
    mainLayout->addLayout(buttonsLayout);
    this->setLayout(mainLayout);
}
