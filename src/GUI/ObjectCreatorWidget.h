#ifndef OBJECTCREATERWIDGET_H
#define OBJECTCREATERWIDGET_H

#include "ObjectRowWidget.h"
#include "DataTransferInterface.h"
#include "DefaultObjects.h"

#include <QWidget>
#include <QListWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QComboBox>
#include <QPushButton>

#include <random>
#include <algorithm>


class ObjectCreatorWidget : public QWidget
{
public:
    explicit ObjectCreatorWidget(QWidget* parent = nullptr);

    std::vector<objectContent> getObjects();

private slots:
    void addButtonClicked();
    void removeButtonClicked();
    void setDefaultButtonClicked();
    void setRandomButtonClicked();

private:
    std::shared_ptr<QListWidget> _objectRowWidgetsList;
    std::shared_ptr<QComboBox> _list;

    size_t _curIndex = 0;

    ObjectRowWidget* getRow(QListWidgetItem* item);
    defaultObject findDefault(const UniversalString& name);
};


#endif //OBJECTCREATERWIDGET_H
