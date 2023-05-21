#ifndef ACTIONWIDGET_H
#define ACTIONWIDGET_H

#include "DataTransferInterface.h"
#include "ObjectRowWidget.h"

#include <QWidget>
#include <QListWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>

#include <vector>


class ActionWidget : public QWidget
{
    Q_OBJECT

public:
    ActionWidget(const std::vector<actionContent>& actions, QWidget* parent = nullptr);

public slots:
    void setActions(const std::vector<actionContent>& actions);

signals:
    void removeActionSignal(int index);
    void addActionSignal(const actionContent& action);
    void updateActionsSignal(const std::vector<actionContent>& actions);

private:
    std::vector<actionContent> _actions;
    std::shared_ptr<QListWidget> _actionList;

    QListWidgetItem* getItem();

private slots:
    void removeSlot(int index);
};


#endif //ACTIONWIDGET_H
