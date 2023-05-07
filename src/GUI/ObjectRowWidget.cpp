#include "ObjectRowWidget.h"

ObjectRowWidget::ObjectRowWidget(const defaultObject& defaultObj, size_t id, QWidget* parent)
        : QWidget(parent), _id(id), _className(defaultObj.className), _varName(_className + QString::number(_id))
{
    auto* mainLayout = new QHBoxLayout(this);

    // class name label
    auto* headerLabel = new QLabel(_className + ":", this);
    headerLabel->setFixedWidth(findMaxClassNameLen() + 1);
    headerLabel->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Fixed);
    mainLayout->addWidget(headerLabel);

    // id label
    auto* indexLabel = new QLabel("id =", this);

    // id line edit
    _idLineEdit.reset(new QLineEdit(QString::number(_id), this));
    _idLineEdit->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
    connect(_idLineEdit.get(), SIGNAL(textChanged(const QString&)), this, SLOT(updateObjectSlot()));
    _idLineEdit->setDisabled(true);

    mainLayout->addLayout( getFormLayout(indexLabel, _idLineEdit.get(), this) );

    // varName label
    auto* varNameLabel = new QLabel("varName =", this);

    // varName line edit
    _varNameLineEdit.reset(new QLineEdit(_varName, this));
    _varNameLineEdit->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
    connect(_varNameLineEdit.get(), SIGNAL(textChanged(const QString&)), this, SLOT(varNameSlot(const QString&)));
    connect(_varNameLineEdit.get(), SIGNAL(textChanged(const QString&)), this, SLOT(updateObjectSlot()));

    mainLayout->addLayout( getFormLayout(varNameLabel, _varNameLineEdit.get(), this) );

    // count label
    auto* countLabel = new QLabel("count =", this);

    // count spin box
    _countSpinBox.reset(new QSpinBox(this));
    connect(_countSpinBox.get(), SIGNAL(valueChanged(int)), this, SLOT(countSlot(int)));
    connect(_countSpinBox.get(), SIGNAL(valueChanged(int)), this, SLOT(updateObjectSlot()));
    _countSpinBox->setRange(1, 1000);
    _countSpinBox->setValue(_count);

    mainLayout->addLayout( getFormLayout(countLabel, _countSpinBox.get(), this) );

    // create label and line edit for each argument with default value
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

        ptr->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
        _fields.emplace_back( ptr.get() );

        mainLayout->addLayout(getFormLayout(label, ptr.release(), this) );
    }

    mainLayout->setAlignment(Qt::AlignLeft);
    mainLayout->setSpacing(60);
    this->setLayout(mainLayout);
}

ObjectRowWidget::ObjectRowWidget(const defaultObject& defaultObj, size_t id, const objectContent& object, QWidget* parent)
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
    object.count  = _count;

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
}

void ObjectRowWidget::updateObjectSlot()
{
    emit updateObjectSignal();
}

QHBoxLayout* ObjectRowWidget::getFormLayout(QLabel* label, QWidget* field, QWidget* parent)
{
    label->setText(label->text() + " ");

    label->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
    field->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);

    auto* layout = new QHBoxLayout(parent);
    layout->addWidget(label);
    layout->addWidget(field);
    layout->setSpacing(0);

    return layout;
}