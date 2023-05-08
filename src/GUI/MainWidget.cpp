#include "MainWidget.h"


MainWidget::MainWidget(MainLogic& logic) : _logic(logic)
{
    // add first config and thread
    _configs.emplace_back();
    _configs.back().emplace_back();

    // create layouts
    auto* mainLayout = new QVBoxLayout(this);
    auto* widgetsLayout = new QHBoxLayout();
    auto* buttonsLayout = new QHBoxLayout();

    // add configs widget
    _configsWidget.reset(new ConfigsWidget(this));

    connect(_configsWidget.get(), SIGNAL(changeConfigIndexSignal(int)), this, SLOT(changeConfigIndexSlot(int)));
    connect(_configsWidget.get(), SIGNAL(addConfigSignal()), this, SLOT(addConfigSlot()));
    connect(_configsWidget.get(), SIGNAL(removeConfigSignal(int)), this, SLOT(removeConfigSlot(int)));

    widgetsLayout->addWidget( _configsWidget.get() );

    // add thread tree widget
    _threadsTreeWidget.reset(new ThreadsTreeWidget({}, this));

    connect(_threadsTreeWidget.get(), SIGNAL(changeThreadIndexSignal(int)), this, SLOT(changeThreadIndexSlot(int)));
    connect(_threadsTreeWidget.get(), SIGNAL(addThreadSignal()), this, SLOT(addThreadSlot()));
    connect(_threadsTreeWidget.get(), SIGNAL(removeThreadSignal(int)), this, SLOT(removeThreadSlot(int)));
    connect(this, SIGNAL(updateThreadTreeSignal(const std::vector<objectContent>&)),
            _threadsTreeWidget.get(), SLOT(updateThreadTreeSlot(const std::vector<objectContent>&)));

    widgetsLayout->addWidget( _threadsTreeWidget.get() );

    // add object creator widget
    _objectCreatorWidget.reset(new ObjectCreatorWidget(this));

    connect(_objectCreatorWidget.get(), SIGNAL(updateThreadSignal(const std::vector<objectContent>&)),
            this, SLOT(updateThreadContentSlot(const std::vector<objectContent>&)));

    widgetsLayout->addWidget( _objectCreatorWidget.get(), 1);

    // create buttons
    int buttonWidth = 200;
    auto* openButton = new QPushButton("Open", this);
    openButton->setFixedWidth(buttonWidth);
    auto* saveButton = new QPushButton("Save", this);
    saveButton->setFixedWidth(buttonWidth);
    auto* runButton = new QPushButton("Run", this);
    runButton->setFixedWidth(buttonWidth);

    // connect buttons
    connect(openButton, SIGNAL(clicked()), this, SLOT(openButtonSlot()));
    connect(saveButton, SIGNAL(clicked()), this, SLOT(saveButtonSlot()));
    connect(runButton, SIGNAL(clicked()), this, SLOT(runButtonSlot()));

    // add widgets to layouts
    buttonsLayout->addWidget(openButton);
    buttonsLayout->addWidget(saveButton);
    buttonsLayout->addWidget(runButton);

    // config main layout
    buttonsLayout->setAlignment(Qt::AlignRight);

    mainLayout->addLayout(widgetsLayout);
    mainLayout->addLayout(buttonsLayout);
    this->setLayout(mainLayout);

    // set window size
    this->setMinimumSize(3300, 600);
    this->resize(this->width(), 1200);
}

void MainWidget::changeThreadIndexSlot(int index)
{
    if (_skipUpdateObjectCreator)
    {
        return;
    }

    _configs[_configIndex][_threadIndex].objects = _objectCreatorWidget->getObjects();
    _threadIndex = index;
    _objectCreatorWidget->setObjects(_configs[_configIndex][_threadIndex].objects);
}

void MainWidget::addThreadSlot()
{
    _configs[_configIndex].push_back({_configs[_configIndex].size(), {}});
}

void MainWidget::removeThreadSlot(int index)
{
    _configs[_configIndex].erase(_configs[_configIndex].begin() + index);

    if (_threadIndex >= _configs[_configIndex].size())
    {
        --_threadIndex;
    }
}

void MainWidget::updateThreadContentSlot(const std::vector<objectContent>& content)
{
    _configs[_configIndex][_threadIndex].objects = content;
    emit updateThreadTreeSignal(content);
}

void MainWidget::changeConfigIndexSlot(int index)
{
    _configs[_configIndex][_threadIndex].objects = _objectCreatorWidget->getObjects();

    _configIndex = index;
    _threadIndex = 0;

    _skipUpdateObjectCreator = true;
    _threadsTreeWidget->setThreads(_configs[_configIndex]);
    _objectCreatorWidget->setObjects(_configs[_configIndex][_threadIndex].objects);
    _skipUpdateObjectCreator = false;
}

void MainWidget::addConfigSlot()
{
    _configs.emplace_back();
    _configs.back().push_back({0, {}});
}

void MainWidget::removeConfigSlot(int index)
{
    _configs.erase(_configs.begin() + index);

    if (_configIndex >= _configs.size())
    {
        _configIndex = _configs.size() - 1;
    }
}

void MainWidget::openButtonSlot()
{

}

void MainWidget::saveButtonSlot()
{

}

void MainWidget::runButtonSlot()
{
    UniversalString fileExtension = ".json";

    UniversalString configName = _configsWidget->getConfigNames()[_configIndex] + fileExtension;

    ConfigParser parser(configName);
    parser.writeConfig(_configs[_configIndex], {});

    _logic.setPaths({configName});
    _logic.runProgramInstances();
}
