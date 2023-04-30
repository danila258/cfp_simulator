#include "MainWidget.h"
#include "UniversalString.h"


MainWidget::MainWidget() : _objectCreatorWidget(new ObjectCreatorWidget(this))
{
    ConfigParser config("/home/danila/cpp/cfp_simulator/src/config.json");
    auto r = config.getThreads();
    auto g = config.getActions();

    config.writeConfig(r, g);

    // create layouts
    auto* mainLayout = new QVBoxLayout(this);
    auto* buttonsLayout = new QHBoxLayout();

    // create buttons
    auto* openButton = new QPushButton("Open", this);
    auto* saveButton = new QPushButton("Save", this);
    auto* runButton = new QPushButton("Run", this);

    // connect buttons
    connect(openButton, SIGNAL(clicked()), this, SLOT(openButtonSlot()));
    connect(saveButton, SIGNAL(clicked()), this, SLOT(saveButtonSlot()));
    connect(runButton, SIGNAL(clicked()), this, SLOT(runButtonSlot()));

    // add widgets to layouts
    mainLayout->addWidget( _objectCreatorWidget.get() );

    buttonsLayout->addWidget(openButton);
    buttonsLayout->addWidget(saveButton);
    buttonsLayout->addWidget(runButton);

    // config main layout
    mainLayout->addLayout(buttonsLayout);
    this->setLayout(mainLayout);

    // set window size
    this->setMinimumSize(1600, 800);
}

void MainWidget::openButtonSlot()
{

}

void MainWidget::saveButtonSlot()
{

}

void MainWidget::runButtonSlot()
{
    ConfigParser parser("test.json");

    std::vector<threadContent> threads;
    threads.emplace_back();
    threads.back().objects = _objectCreatorWidget->getObjects();

    parser.writeConfig(threads, {});

    std::vector<UniversalString> paths = {"cfp_simulator", "test.json"};

    MainLogic mainLogic(paths, "cfp_simulator");
    mainLogic.run();
}
