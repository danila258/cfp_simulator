#ifndef OBJECTCREATERWIDGET_H
#define OBJECTCREATERWIDGET_H

#include "ObjectRowWidget.h"
#include "DataTransferInterface.h"
#include "DefaultObjects.h"

#include <QWidget>
#include <QListWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSpacerItem>
#include <QComboBox>
#include <QPushButton>
#include <QDebug>

#include <random>
#include <algorithm>


class ObjectCreatorWidget : public QWidget
{
Q_OBJECT

public:
    explicit ObjectCreatorWidget(QWidget* parent = nullptr);

    void setObjects(const std::vector<objectContent>& objects);
    std::vector<objectContent> getObjects();

signals:
    void updateThreadSignal(const std::vector<objectContent>& content);

public slots:
    void updateCountSlot();

private slots:
    void addButtonSlot();
    void removeButtonSlot();
    void setDefaultButtonSlot();
    void setRandomButtonSlot();
    void userInputSlot();

private:
    std::shared_ptr<QListWidget> _objectRowWidgetsList;
    std::shared_ptr<QComboBox> _list;

    size_t _curId = 0;

    QListWidgetItem* getItem();
    ObjectRowWidget* getRow(QListWidgetItem* item);
    static defaultObject findDefault(const UniversalString& name);
};


#endif //OBJECTCREATERWIDGET_H
