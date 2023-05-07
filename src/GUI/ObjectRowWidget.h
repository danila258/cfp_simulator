#ifndef OBJECTROW_H
#define OBJECTROW_H

#include "DataTransferInterface.h"
#include "DefaultObjects.h"
#include "UniversalString.h"

#include <QWidget>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QSpinBox>
#include <QComboBox>
#include <QLabel>
#include <QDebug>
#include <QFontMetrics>
#include <QFont>

#include <vector>
#include <memory>
#include <algorithm>


int static findMaxClassNameLen()
{
    QFont font;
    QFontMetrics metric(font);
    int maxPixels = 0;

    for (auto& obj : gui::defaultObjects)
    {
        int pixels = metric.width(obj.className + ":");

        if (pixels > maxPixels)
        {
            maxPixels = pixels;
        }
    }

    return maxPixels;
}


class ObjectRowWidget : public QWidget
{
Q_OBJECT

public:
    ObjectRowWidget(const defaultObject& defaultObj, size_t id, QWidget* parent = nullptr);
    ObjectRowWidget(const defaultObject& defaultObj, size_t id, const objectContent& object, QWidget* parent = nullptr);

    objectContent getUserInput() const;
    UniversalString getClassName() const;
    size_t getCount() const;

    void setValues(const std::vector<UniversalString>& values);
    void setId(size_t id);

signals:
    void updateObjectSignal();
    void updateCountSignal();

private slots:
    void varNameSlot(const QString& varName);
    void countSlot(int count);
    void updateObjectSlot();

private:
    size_t _id;
    size_t _count = 1;
    UniversalString _className;
    UniversalString _varName;

    std::shared_ptr<QLineEdit> _idLineEdit;
    std::shared_ptr<QLineEdit> _varNameLineEdit;
    std::shared_ptr<QSpinBox> _countSpinBox;

    std::vector<std::shared_ptr<QWidget>> _fields;

    QHBoxLayout* getFormLayout(QLabel* label, QWidget* field, QWidget* parent = nullptr);
};


#endif //OBJECTROW_H