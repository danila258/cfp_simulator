#include "ObjectRowWidget.h"

ObjectRowWidget::ObjectRowWidget(const defaultObject& defaultObj, size_t id, QWidget* parent)
        : QWidget(parent), _id(id), _className(defaultObj.className), _varName(_className + QString::number(_id))
{
    auto* mainLayout = new QHBoxLayout(this);

    // class name label
    auto* headerLabel = new QLabel(defaultObj.className + ":   ", this);
    mainLayout->addWidget(headerLabel);

    // id label
    auto* indexLabel = new QLabel("id =", this);
    mainLayout->addWidget(indexLabel);

    // id line edit
    _idLineEdit.reset(new QLineEdit(QString::number(_id), this));
    _idLineEdit->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
    connect(_idLineEdit.get(), SIGNAL(textChanged(const QString&)), this, SLOT(updateObjectSlot()));
    _idLineEdit->setDisabled(true);
    mainLayout->addWidget( _idLineEdit.get() );

    // varName label
    auto* varNameLabel = new QLabel("varName =", this);
    mainLayout->addWidget(varNameLabel);

    // varName line edit
    _varNameLineEdit.reset(new QLineEdit(_varName, this));
    _varNameLineEdit->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
    connect(_varNameLineEdit.get(), SIGNAL(textChanged(const QString&)), this, SLOT(varNameSlot(const QString&)));
    connect(_varNameLineEdit.get(), SIGNAL(textChanged(const QString&)), this, SLOT(updateObjectSlot()));
    mainLayout->addWidget( _varNameLineEdit.get() );

    // count spin box
    _countSpinBox.reset(new QSpinBox(this));
    connect(_countSpinBox.get(), SIGNAL(valueChanged(int)), this, SLOT(countSlot(int)));
    connect(_countSpinBox.get(), SIGNAL(valueChanged(int)), this, SLOT(updateObjectSlot()));
    _countSpinBox->setRange(1, 1000);
    _countSpinBox->setValue(_count);
    mainLayout->addWidget( _countSpinBox.get() );

    // create label and line edit for each argument with default value
    for (const auto& field : defaultObj.fields)
    {
        auto* label = new QLabel(field.name + "=", this);
        mainLayout->addWidget(label);

        std::unique_ptr<QWidget> ptr;

        if (field.type == "int")
        {
            ptr.reset(new QSpinBox(this));
            auto* spinBox = qobject_cast<QSpinBox*>( ptr.get() );

            connect(spinBox, SIGNAL(valueChanged(int)), this, SLOT(updateObjectSlot()));
            spinBox->setRange(0, 1000);
            spinBox->setValue( field.val->toInt() );
        }
        else if (field.type == "bool")
        {
            ptr.reset(new QComboBox(this));
            auto* comboBox = qobject_cast<QComboBox*>( ptr.get() );

            connect(comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(updateObjectSlot()));
            comboBox->addItem("false");
            comboBox->addItem("true");
            comboBox->setCurrentIndex( field.val->toInt() );
        }
        else
        {
            ptr.reset(new QLineEdit(field.val, this));
            connect(qobject_cast<QLineEdit*>( ptr.get() ), SIGNAL(textChanged(const QString&)),
                    this, SLOT(updateObjectSlot()));
        }

        ptr->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
        mainLayout->addWidget( ptr.get() );
        _fields.emplace_back( std::move(ptr) );
    }

    // add spacer to the end of the main layout
    auto* spacer = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);
    mainLayout->addItem(spacer);

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

    for (auto& i : values)
    {
        qDebug() << i;
    }

    for (const auto& field : _fields)
    {
        UniversalString className = field->metaObject()->className();

        if (className == "QSpinBox")
        {
            qDebug() << values[index];
            qobject_cast<QSpinBox*>(field.get())->setValue( std::stoi(values[index]) );
        }
        else if (className == "QComboBox")
        {
            qDebug() << values[index];
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