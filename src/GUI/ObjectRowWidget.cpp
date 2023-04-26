#include "ObjectRowWidget.h"

#include <utility>

ObjectRowWidget::ObjectRowWidget(RTLibObject defaultObject, QWidget* parent)
        : QWidget(parent), _defaultObject(std::move(defaultObject)), _varName(_defaultObject.varName)
{
    auto* mainLayout = new QHBoxLayout(this);

    // class name label
    auto* headerLabel = new QLabel(_defaultObject.className + ":   ", this);
    mainLayout->addWidget(headerLabel);

    // id label
    auto* indexLabel = new QLabel("id =", this);
    mainLayout->addWidget(indexLabel);

    // id line edit
    _idLineEdit = new QLineEdit(QString::number(_defaultObject.id), this);
    _idLineEdit->setDisabled(true);
    mainLayout->addWidget(_idLineEdit);

    // varName label
    auto* varNameLabel = new QLabel("varName =", this);
    mainLayout->addWidget(varNameLabel);

    // varName line edit
    _varNameLineEdit = new QLineEdit(_defaultObject.varName, this);
    connect(_varNameLineEdit, SIGNAL(textChanged(const QString&)), this, SLOT(varNameChanged(const QString&)));
    mainLayout->addWidget(_varNameLineEdit);

    // create label and line edit for each argument with default value
    for (const auto& pair : _defaultObject.argVal)
    {
        auto* label = new QLabel(pair.first + "=", this);
        mainLayout->addWidget(label);

        auto* lineEdit = new QLineEdit(pair.second, this);
        mainLayout->addWidget(lineEdit);

        _lineEdits.push_back(lineEdit);
    }

    this->setLayout(mainLayout);
}

ObjectRowWidget::~ObjectRowWidget()
{
    delete _idLineEdit;
    delete _varNameLineEdit;

    for (auto* item : _lineEdits)
    {
        delete item;
    }
}

RTLibObject ObjectRowWidget::getUserInput() const
{
    RTLibObject input = _defaultObject;

    for (int i = 0; i < _lineEdits.size(); ++i)
    {
        input.argVal[i].second = _lineEdits[i]->text();
    }

    return input;
}

QString ObjectRowWidget::getClassName() const
{
    return _defaultObject.className;
}

void ObjectRowWidget::setValues(const QVector<QString>& values)
{
    for (int i = 0; i < _lineEdits.size(); ++i)
    {
        _lineEdits[i]->setText( values.at(i) );
    }
}

void ObjectRowWidget::setId(int id)
{
    // replace id in varName to new
    if ( _varName.endsWith(QString::number(_defaultObject.id )) )
    {
        int idLen = QString::number(_defaultObject.id).size();

        // delete id from varName
        for (int i = 0; i < idLen; ++i)
        {
            _varName.remove(_varName.size() - 1, 1);  // remove last char
        }

        _varName += QString::number(id);
        _varNameLineEdit->setText(_varName);
    }

    _idLineEdit->setText(QString::number(id));
    _defaultObject.id = id;
}

void ObjectRowWidget::varNameChanged(const QString& varName)
{
    _varName = varName;
}
