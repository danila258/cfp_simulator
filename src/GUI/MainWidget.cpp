#include "MainWidget.h"

MainWidget::MainWidget(MainLogic& logic) : _logic(logic)
{
    // add first config, thread and action
    _configs.emplace_back();
    _configs.back().emplace_back();
    _actions.emplace_back();

    // create layouts
    auto* mainLayout = new QVBoxLayout(this);
    auto* widgetsLayout = new QHBoxLayout();
    auto* buttonsLayout = new QHBoxLayout();

    // add configs widget
    _configsWidget.reset(new ConfigsWidget(this));

    connect(_configsWidget.get(), SIGNAL(changeConfigIndexSignal(int)), this, SLOT(changeConfigIndexSlot(int)));
    connect(_configsWidget.get(), SIGNAL(addConfigSignal()), this, SLOT(addConfigSlot()));
    connect(_configsWidget.get(), SIGNAL(removeConfigSignal(int)), this, SLOT(removeConfigSlot(int)));
    connect(this, SIGNAL(addConfigSignal(const QString&)), _configsWidget.get(), SLOT(addConfigSlot(const QString&)));

    widgetsLayout->addWidget( _configsWidget.get() );

    // add thread tree widget
    _threadsTreeWidget.reset(new ThreadsTreeWidget({}, this));

    connect(_threadsTreeWidget.get(), SIGNAL(changeThreadIndexSignal(int)), this, SLOT(changeThreadIndexSlot(int)));
    connect(_threadsTreeWidget.get(), SIGNAL(addThreadSignal()), this, SLOT(addThreadSlot()));
    connect(_threadsTreeWidget.get(), SIGNAL(removeThreadSignal(int)), this, SLOT(removeThreadSlot(int)));
    connect(this, SIGNAL(updateThreadTreeSignal(const std::vector<objectContent>&)),
            _threadsTreeWidget.get(), SLOT(updateThreadTreeSlot(const std::vector<objectContent>&)));

    widgetsLayout->addWidget( _threadsTreeWidget.get() );

    // add tab widget
    auto* tabWidget = new QTabWidget(this);
    widgetsLayout->addWidget(tabWidget, 1);

    connect(tabWidget, SIGNAL(currentChanged(int)), this, SLOT(changeTabSlot(int)));

    // add object creator widget
    _objectCreatorWidget.reset(new ObjectCreatorWidget(_threadIndex, this));

    connect(_objectCreatorWidget.get(), SIGNAL(updateThreadSignal(const std::vector<objectContent>&)),
            this, SLOT(updateThreadContentSlot(const std::vector<objectContent>&)));
    connect(_objectCreatorWidget.get(), SIGNAL(addActionSignal(const actionContent&)),
            this, SLOT(addActionSlot(const actionContent&)));

    tabWidget->addTab(_objectCreatorWidget.get(), "Objects");

    // add action widget
    _actionWidget.reset( new ActionWidget(_actions[_configIndex], this) );
    tabWidget->addTab(_actionWidget.get(), "Actions");

    connect(this, SIGNAL(updateActionWidgetSignal(const std::vector<actionContent>&)),
            _actionWidget.get(), SLOT(setActionsSlot(const std::vector<actionContent>&)));
    connect(_actionWidget.get(), SIGNAL(removeActionSignal(int)), this, SLOT(removeActionSlot(int)));

    // create buttons
    int buttonWidth = QApplication::primaryScreen()->availableSize().width() / 12;

    auto* openButton = new QPushButton("Open...", this);
    openButton->setFixedWidth(buttonWidth);
    auto* saveButton = new QPushButton("Save...", this);
    saveButton->setFixedWidth(buttonWidth);
    auto* runButton = new QPushButton("Run...", this);
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

    // load settings
    loadSettings();
}

void MainWidget::changeThreadIndexSlot(int index)
{
    if (_skipUpdateObjectCreator)
    {
        return;
    }

    _configs[_configIndex][_threadIndex].objects = _objectCreatorWidget->getObjects();
    _threadIndex = index;
    _objectCreatorWidget->setObjects(_threadIndex, _configs[_configIndex][_threadIndex].objects);
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
    _objectCreatorWidget->setObjects(_threadIndex, _configs[_configIndex][_threadIndex].objects);
    _skipUpdateObjectCreator = false;

    emit updateActionWidgetSignal(_actions[_configIndex]);
}

void MainWidget::addConfigSlot()
{
    _configs.emplace_back();
    _configs.back().push_back({0, {}});
    _actions.emplace_back();
}

void MainWidget::removeConfigSlot(int index)
{
    _configs.erase(_configs.begin() + index);
    _actions.erase(_actions.begin() + index);

    if (_configIndex >= _configs.size())
    {
        _configIndex = _configs.size() - 1;
    }
}

void MainWidget::addActionSlot(const actionContent& action)
{
    _actions[_configIndex].emplace_back(action);
}

void MainWidget::removeActionSlot(int index)
{
    _actions[_configIndex].erase(_actions[_configIndex].begin() + index);
}

void MainWidget::updateActionsSlot(const std::vector<actionContent>& actions)
{
    _actions[_configIndex] = actions;
}

void MainWidget::changeTabSlot(int index)
{
    emit updateActionWidgetSignal(_actions[_configIndex]);
}

void MainWidget::openButtonSlot()
{
    QStringList paths = QFileDialog::getOpenFileNames(this, tr("Open Files"), "/home", "JSON (*.json)");

    for (auto& path : paths)
    {
        try
        {
            ConfigParser parser(path);
            _configs.emplace_back( parser.getThreads() );
            _actions.emplace_back( parser.getActions() );

            emit addConfigSignal( path.mid(path.lastIndexOf("/") + 1).remove(".json") );
        }
        catch(...)
        {
            QErrorMessage errorMessage;
            errorMessage.showMessage("Can't open file:  " + path);
            errorMessage.exec();
        }
    }
}

void MainWidget::saveButtonSlot()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"), "/home",
                                                    QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    if ( dir.isEmpty() )
    {
        return;
    }

    ConfigParser parser(dir + "/" + _configsWidget->getConfigName(_configIndex) + ".json");
    parser.writeConfig(_configs[_configIndex], _actions[_configIndex]);
}

void MainWidget::runButtonSlot()
{
    UniversalString fileExtension = ".json";
    UniversalString configName = _configsWidget->getConfigName(_configIndex) + fileExtension;

    std::unique_ptr<QMessageBox> logDialog( new QMessageBox(QMessageBox::Question,  "Log", "Do you want to save log?",
                                       QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel, this) );
    int result = logDialog->exec();

    if (result == QMessageBox::Yes)
    {
        _logic.setLoggingState(true);
    }
    else if (result == QMessageBox::No)
    {
        _logic.setLoggingState(false);
    }
    else
    {
        return;
    }

    ConfigParser parser(configName);
    parser.writeConfig(_configs[_configIndex], _actions[_configIndex]);

    _logic.setPaths( {configName} );
    _logic.runProgramInstances();
}

void MainWidget::loadSettings()
{
    QSize fullScreenSize = QApplication::primaryScreen()->size();

    _settings.beginGroup("application condition");
    QSize appSize = _settings.value("window size", fullScreenSize).toSize();
    QPoint appPos = _settings.value("window position", QPoint(0, 0)).toPoint();
    _settings.endGroup();

    resize(appSize);
    move(appPos);
}

void MainWidget::closeEvent(QCloseEvent* event)
{
    _settings.beginGroup("application condition");
    _settings.setValue("window size", size());
    _settings.setValue("window position", pos());
    _settings.endGroup();
}
