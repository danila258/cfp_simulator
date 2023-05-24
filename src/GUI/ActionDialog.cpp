#include "ActionDialog.h"

ActionDialog::ActionDialog(size_t thread, const UniversalString& className, const std::vector<defaultParam>& defaultActions,
    size_t minId, size_t maxId, QWidget* parent) : QDialog(parent), _defaultActions(defaultActions), _thread(thread),
    _className(className), _maxId(maxId), _minId(minId), _funcName(defaultActions.front().name), _id(minId)
{
    // layouts
    auto* mainLayout = new QVBoxLayout(this);
    auto* topLayout = new QHBoxLayout();
    _actionLayout.reset( new QVBoxLayout() );

    // id label
    auto* idLabel = new QLabel("id =", this);

    // id spin box
    auto* idSpinBox = new QSpinBox(this);
    idSpinBox->setRange(_minId, _maxId);
    connect(idSpinBox, SIGNAL(valueChanged(int)), this, SLOT(idSlot(int)));

    // pause label
    auto* pauseLabel = new QLabel("Pause =", this);

    // pause spinBox
    auto* pauseSpinBox = new QSpinBox(this);
    pauseSpinBox->setSuffix(" ms");
    pauseSpinBox->setRange(0, 100000);
    pauseSpinBox->setSingleStep(100);
    pauseSpinBox->setValue(_pause);

    connect(pauseSpinBox, SIGNAL(valueChanged(int)), this, SLOT(pauseSlot(int)));

    // action comboBox
    auto* actionComboBox = new QComboBox(this);
    connect(actionComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(actionSlot(int)));

    for (auto& action : defaultActions)
    {
        actionComboBox->addItem(action.name);
    }

    // action row
    _actionRow.reset( new ObjectRowWidget(_defaultActions.front(), this) );

    // dialog buttons
    auto* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    // setting layouts
    auto* groupBox = new QGroupBox("Arguments");

    topLayout->setSpacing(60);
    topLayout->setAlignment(Qt::AlignLeft);
    topLayout->addLayout( getFormLayout(idLabel, idSpinBox) );
    topLayout->addLayout( getFormLayout(pauseLabel, pauseSpinBox) );
    topLayout->addWidget(actionComboBox);

    _actionLayout->addWidget( _actionRow.get() );
    groupBox->setLayout( _actionLayout.get() );

    mainLayout->addLayout(topLayout);
    mainLayout->addWidget(groupBox);
    mainLayout->addWidget(buttonBox);

    // dialog setting
    setWindowTitle(className);
    setModal(true);
}

actionContent ActionDialog::getAction()
{
    actionContent action;
    action.thread = _thread;
    action.action = "call";
    action.className = _className;
    action.id = _id;
    action.pause = _pause;
    action.funcName = _funcName;
    action.args = _actionRow->getUserInput().args;

    return action;
}

void ActionDialog::actionSlot(int index)
{
    if (_actionRow == nullptr)
    {
        return;
    }

    _actionLayout->removeWidget( _actionRow.get() );
    _actionRow.reset( new ObjectRowWidget(_defaultActions.front(), this) );
    _actionLayout->addWidget( _actionRow.get() );

    _funcName = _defaultActions[index].name;
}

void ActionDialog::pauseSlot(int value)
{
    _pause = value;
}

void ActionDialog::idSlot(int value)
{
    _id = value;
}

QHBoxLayout* ActionDialog::getFormLayout(QLabel* label, QWidget* field, QWidget* parent)
{
    label->setText(label->text() + " ");

    label->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    field->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    auto* layout = new QHBoxLayout(parent);
    layout->addWidget(label);
    layout->addWidget(field);
    layout->setSpacing(0);

    return layout;
}

