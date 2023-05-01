#include "MainWidget.h"


MainWidget::MainWidget(MainLogic& logic) : _logic(logic)
{
    // create layouts
    auto* mainLayout = new QVBoxLayout(this);
    auto* widgetsLayout = new QHBoxLayout();
    auto* buttonsLayout = new QHBoxLayout();

    // add configs widget
    _configsWidget.reset(new ConfigsWidget(this));
    widgetsLayout->addWidget( _configsWidget.get() );

    // add thread tree widget
    _threadsTreeWidget.reset(new ThreadsTreeWidget({}, this));
    widgetsLayout->addWidget( _threadsTreeWidget.get() );

    // add object creator widget
    _objectCreatorWidget.reset(new ObjectCreatorWidget(this));
    widgetsLayout->addWidget( _objectCreatorWidget.get() );

    // create buttons
    auto* openButton = new QPushButton("Open", this);
    auto* saveButton = new QPushButton("Save", this);
    auto* runButton = new QPushButton("Run", this);

    // connect buttons
    connect(openButton, SIGNAL(clicked()), this, SLOT(openButtonSlot()));
    connect(saveButton, SIGNAL(clicked()), this, SLOT(saveButtonSlot()));
    connect(runButton, SIGNAL(clicked()), this, SLOT(runButtonSlot()));

    // add widgets to layouts
    buttonsLayout->addWidget(openButton);
    buttonsLayout->addWidget(saveButton);
    buttonsLayout->addWidget(runButton);

    // config main layout
    mainLayout->addLayout(widgetsLayout);
    mainLayout->addLayout(buttonsLayout);
    this->setLayout(mainLayout);

    // set window size
    this->setMinimumSize(1600, 800);
}

void MainWidget::changeThreadSlot(int index)
{
    _threadsContent[_threadIndex] = _objectCreatorWidget->getObjects();
    _objectCreatorWidget->setObjects(_threadsContent[index]);
    _threadIndex = index;
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

    std::vector<UniversalString> paths = {"test.json"};

    _logic.setPaths(paths);
    _logic.runProgramInstances();
}
