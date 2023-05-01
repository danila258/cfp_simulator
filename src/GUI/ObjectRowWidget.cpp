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
    _idLineEdit->setDisabled(true);
    mainLayout->addWidget( _idLineEdit.get() );

    // varName label
    auto* varNameLabel = new QLabel("varName =", this);
    mainLayout->addWidget(varNameLabel);

    // varName line edit
    _varNameLineEdit.reset(new QLineEdit(_varName, this));
    connect(_varNameLineEdit.get(), SIGNAL(textChanged(const QString&)), this, SLOT(varNameChanged(const QString&)));
    mainLayout->addWidget( _varNameLineEdit.get() );

    // create label and line edit for each argument with default value
    for (const auto& field : defaultObj.fields)
    {
        auto* label = new QLabel(field.name + "=", this);
        mainLayout->addWidget(label);

        std::unique_ptr<QWidget> ptr;

        if (field.type == "int")
        {
            ptr.reset(new QSpinBox(this));

            qobject_cast<QSpinBox*>(ptr.get())->setRange(0, 1000);
            qobject_cast<QSpinBox*>(ptr.get())->setValue( field.val->toInt() );
        }
        else if (field.type == "bool")
        {
            ptr.reset(new QComboBox(this));

            qobject_cast<QComboBox*>(ptr.get())->addItem("false");
            qobject_cast<QComboBox*>(ptr.get())->addItem("true");
            qobject_cast<QComboBox*>(ptr.get())->setCurrentIndex( field.val->toInt() );
        }
        else
        {
            ptr.reset(new QLineEdit(field.val, this));
        }

        mainLayout->addWidget( ptr.get() );
        _fields.emplace_back( std::move(ptr) );
    }

    // count spin box
    _countSpinBox.reset(new QSpinBox(this));
    connect(_countSpinBox.get(), SIGNAL(valueChanged(int)), this, SLOT(countChanged(int)));
    _countSpinBox->setRange(1, 1000);
    _countSpinBox->setValue(_count);
    mainLayout->addWidget( _countSpinBox.get() );

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
            object.args.emplace_back( QString(qobject_cast<QSpinBox*>(field.get())->value()) );
        }
        else if (className == "QComboBox")
        {
            object.args.emplace_back( QString(qobject_cast<QComboBox*>(field.get())->currentIndex()) );
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

void ObjectRowWidget::varNameChanged(const QString& varName)
{
    _varName = varName;
}

void ObjectRowWidget::countChanged(int count)
{
    _count = count;
    emit updateCountSignal();
}