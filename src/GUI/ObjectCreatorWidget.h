#ifndef OBJECTCREATERWIDGET_H
#define OBJECTCREATERWIDGET_H

#include "ObjectRowWidget.h"
#include "ActionDialog.h"
#include "DataTransferInterface.h"
#include "DefaultObjects.h"

#include <QWidget>
#include <QListWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSpacerItem>
#include <QComboBox>
#include <QPushButton>
#include <QScrollBar>

#include <random>
#include <algorithm>


class ObjectCreatorWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ObjectCreatorWidget(size_t thread, QWidget* parent = nullptr);

    void setObjects(size_t thread, const std::vector<objectContent>& objects);
    std::vector<objectContent> getObjects();

signals:
    void updateThreadSignal(const std::vector<objectContent>& content);
    void addActionSignal(const actionContent& action);

public slots:
    void updateCountSlot();

private slots:
    void addObjectSlot();
    void addActionSlot();
    void removeObjectSlot();
    void setDefaultSlot();
    void setRandomSlot();
    void userInputSlot();

private:
    std::shared_ptr<QListWidget> _objectRowWidgetList;
    std::shared_ptr<QComboBox> _classNamelist;

    size_t _thread;
    size_t _id = 0;

    QListWidgetItem* getItem();
    ObjectRowWidget* getRow(QListWidgetItem* item);
    static defaultParam findDefault(const UniversalString& name);
};


#endif //OBJECTCREATERWIDGET_H
