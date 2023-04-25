#include "MainWidget.h"

MainWidget::MainWidget()
{
    QVector<QPair<QString, QVector<QPair<QString, QString>>>> def {{"TRTMutex", {{"lock", "true"}, {"ccc", "67"}}},
                                                                   {"TRTCondition", {{"wait", "false"}}}};

    _objectCreatorWidget = new ObjectCreatorWidget(def, this);

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
