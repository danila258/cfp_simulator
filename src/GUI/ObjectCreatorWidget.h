#ifndef OBJECTCREATERWIDGET_H
#define OBJECTCREATERWIDGET_H

#include "ObjectRowWidget.h"

#include <QWidget>
#include <QListWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QComboBox>
#include <QPushButton>

#include <random>


class ObjectCreatorWidget : public QWidget
{
public:
    explicit ObjectCreatorWidget(const QVector<QPair<QString, QVector<QPair<QString, QString>>>>& defaultObjects, QWidget* parent = nullptr);
    ~ObjectCreatorWidget() override;

    QVector<RTLibObject> getObjects();

private slots:
    void addButtonClicked();
    void removeButtonClicked();
    void setDefaultButtonClicked();
    void setRandomButtonClicked();

private:
    QVector<QPair<QString, QVector<QPair<QString, QString>>>> _defaultObjects;

    QListWidget* _objectRowWidgetsList;
    QComboBox* _list;

    int _curIndex = 0;
};


#endif //OBJECTCREATERWIDGET_H
