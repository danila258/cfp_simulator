#include "ObjectRowWidget.h"

ObjectRowWidget::ObjectRowWidget(const defaultParam& defaultObj, size_t id, QWidget* parent)
        : QWidget(parent), _id(id), _className(defaultObj.name), _varName(_className + QString::number(_id))
{
    auto* mainLayout = new QHBoxLayout(this);

    // class name label
    auto* headerLabel = new QLabel(_className + ":");
    headerLabel->setFixedWidth(findMaxClassNameLen() + 1);
    mainLayout->addWidget(headerLabel);

    // id spin box
    _idLineEdit.reset(new QLineEdit(QString::number(_id)));
    connect(_idLineEdit.get(), SIGNAL(textChanged(const QString&)), this, SLOT(updateObjectSlot()));
    _idLineEdit->setDisabled(true);

    // id label
    auto* indexLabel = new QLabel("id =");
    mainLayout->addLayout( getFormLayout(indexLabel, _idLineEdit.get()) );

    // varName line edit
    _varNameLineEdit.reset(new QLineEdit(_varName));
    connect(_varNameLineEdit.get(), SIGNAL(textChanged(const QString&)), this, SLOT(varNameSlot(const QString&)));
    connect(_varNameLineEdit.get(), SIGNAL(textChanged(const QString&)), this, SLOT(updateObjectSlot()));

    // varName label
    auto* varNameLabel = new QLabel("varName =");
    mainLayout->addLayout( getFormLayout(varNameLabel, _varNameLineEdit.get()) );

    // count spin box
    _countSpinBox.reset(new QSpinBox(this));
    connect(_countSpinBox.get(), SIGNAL(valueChanged(int)), this, SLOT(countSlot(int)));
    connect(_countSpinBox.get(), SIGNAL(valueChanged(int)), this, SLOT(updateObjectSlot()));
    _countSpinBox->setRange(1, 1000);
    _countSpinBox->setValue(_count);

    // count label
    auto* countLabel = new QLabel("count =", this);
    mainLayout->addLayout( getFormLayout(countLabel, _countSpinBox.get()) );

    // create label and line edit for each argument with default value
    addDefaultFields(mainLayout, defaultObj);
}

ObjectRowWidget::ObjectRowWidget(const defaultParam& defaultObj, QWidget* parent)
{
    auto* mainLayout = new QHBoxLayout(this);

    // create label and line edit for each argument with default value
    addDefaultFields(mainLayout, defaultObj);
}

ObjectRowWidget::ObjectRowWidget(const actionContent& action, QWidget* parent)
{
    auto* mainLayout = new QHBoxLayout(this);

    // class name label
    auto* classNameLabel = new QLabel(action.className + ":");
    classNameLabel->setFixedWidth(findMaxClassNameLen() + 1);
    mainLayout->addWidget(classNameLabel);

    // thread label
    auto* threadLabel = new QLabel("thread =");

    // thread spin box
    auto threadSpinBox = new QSpinBox(this);
    threadSpinBox->setValue(action.thread);
    threadSpinBox->setDisabled(true);

    // id label
    auto* idLabel = new QLabel("id =");

    // id spin box
    auto idSpinBox = new QSpinBox(this);
    idSpinBox->setValue(action.id);
    idSpinBox->setDisabled(true);

    // action label
    auto* actionLabel = new QLabel("action =");

    // action line edit
    auto* actionLineEdit = new QLineEdit(action.action);
    actionLineEdit->setDisabled(true);

    // pause label
    auto* pauseLabel = new QLabel("pause =");

    // pause spin box
    auto* pauseSpinBox = new QSpinBox(this);

    pauseSpinBox->setSuffix(" ms");
    pauseSpinBox->setRange(0, 100000);
    pauseSpinBox->setSingleStep(100);
    pauseSpinBox->setValue(action.pause);
    pauseSpinBox->setDisabled(true);

    // function name label
    auto* functionNameLabel = new QLabel("functionName =");

    // function name line edit
    auto* functionNameLineEdit = new QLineEdit(action.funcName);
    functionNameLineEdit->setDisabled(true);

    mainLayout->addLayout( getFormLayout(threadLabel, threadSpinBox) );
    mainLayout->addLayout( getFormLayout(idLabel, idSpinBox) );
    mainLayout->addLayout( getFormLayout(actionLabel, actionLineEdit) );
    mainLayout->addLayout( getFormLayout(pauseLabel, pauseSpinBox) );
    mainLayout->addLayout( getFormLayout(functionNameLabel, functionNameLineEdit) );
}

ObjectRowWidget::ObjectRowWidget(const defaultParam& defaultObj, size_t id, const objectContent& object, QWidget* parent)
    : ObjectRowWidget(defaultObj, id, parent)
{
    _varName = object.varName;
    _varNameLineEdit->setText(_varName);

    _count = object.count;
    _countSpinBox->setValue(object.count);

    setValues(object.args);
}

objectContent ObjectRowWidget::getUserInput() const
{
    objectContent object;
    object.className = _className;
    object.varName = _varName;
    object.startId = _id;
    object.count = _count;

    for (const auto& field : _fields)
    {
        UniversalString className = field->metaObject()->className();

        if (className == "QSpinBox")
        {
            object.args.emplace_back( QString::number(qobject_cast<QSpinBox*>(field.get())->value()) );
        }
        else if (className == "QComboBox")
        {
            object.args.emplace_back( QString::number(qobject_cast<QComboBox*>(field.get())->currentIndex()) );
        }
        else if (className == "QLineEdit")
        {
            object.args.emplace_back( qobject_cast<QLineEdit*>(field.get())->text() );
        }
    }

    return object;
}

UniversalString ObjectRowWidget::getClassName() const
{
    return _className;
}

size_t ObjectRowWidget::getId() const
{
    return _id;
}

size_t ObjectRowWidget::getCount() const
{
    return _count;
}

void ObjectRowWidget::setValues(const std::vector<UniversalString>& values)
{
    size_t index = 0;

    for (const auto& field : _fields)
    {
        UniversalString className = field->metaObject()->className();

        if (className == "QSpinBox")
        {
            qobject_cast<QSpinBox*>(field.get())->setValue( std::stoi(values[index]) );
        }
        else if (className == "QComboBox")
        {
            qobject_cast<QComboBox*>(field.get())->setCurrentIndex(std::stoi(values[index]) % 2);
        }
        else if (className == "QLineEdit")
        {
            qobject_cast<QLineEdit*>(field.get())->setText(values[index]);
        }

        ++index;
    }
}

void ObjectRowWidget::setId(size_t id)
{
    // replace id in varName to new
    if ( _varName->endsWith(QString::number(_id)) )
    {
        int idLen = QString::number(_id).size();

        // delete id from varName
        for (int i = 0; i < idLen; ++i)
        {
            _varName->remove(_varName->size() - 1, 1);  // remove last char
        }

        _varName->append( QString::number(id) );
        _varNameLineEdit->setText(_varName);
    }

    _idLineEdit->setText( QString::number(id) );
    _id = id;
}

void ObjectRowWidget::varNameSlot(const QString& varName)
{
    _varName = varName;
}

void ObjectRowWidget::countSlot(int count)
{
    _count = count;
    emit updateCountSignal();
    emit updateObjectSignal();
}

void ObjectRowWidget::updateObjectSlot()
{
    emit updateObjectSignal();
}

QHBoxLayout* ObjectRowWidget::getFormLayout(QLabel* label, QWidget* field, QWidget* parent)
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

void ObjectRowWidget::addDefaultFields(QHBoxLayout* mainLayout, const defaultParam& defaultObj)
{
    for (const auto& field : defaultObj.fields)
    {
        auto* label = new QLabel(field.name + "=", this);
        std::unique_ptr<QWidget> ptr;

        if (field.type == "int")
        {
            auto* spinBox = new QSpinBox(this);
            ptr.reset(spinBox);

            connect(spinBox, SIGNAL(valueChanged(int)), this, SLOT(updateObjectSlot()));
            spinBox->setRange(0, 1000);
            spinBox->setValue( field.val->toInt() );
        }
        else if (field.type == "bool")
        {
            auto* comboBox = new QComboBox(this);
            ptr.reset(comboBox);

            connect(comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(updateObjectSlot()));
            comboBox->addItem("false");
            comboBox->addItem("true");
            comboBox->setCurrentIndex( field.val->toInt() );
        }
        else
        {
            auto* lineEdit = new QLineEdit(field.val, this);
            ptr.reset(lineEdit);

            connect(lineEdit, SIGNAL(textChanged(const QString&)),this, SLOT(updateObjectSlot()));
            lineEdit->setText(_varName);
        }

        _fields.emplace_back( ptr.get() );
        mainLayout->addLayout(getFormLayout(label, ptr.release()) );
    }

    mainLayout->setAlignment(Qt::AlignLeft);
    mainLayout->setSpacing(60);

    this->setLayout(mainLayout);
}