#ifndef CONFIGSWIDGET_H
#define CONFIGSWIDGET_H

#include "UniversalString.h"
#include "DataTransferInterface.h"

#include <QWidget>
#include <QListWidget>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDebug>

#include <memory>


class ConfigsWidget : public QWidget
{
    Q_OBJECT

public:
    ConfigsWidget(QWidget* parent = nullptr);

    std::vector<UniversalString> getConfigNames();

signals:
    void changeConfigIndexSignal(int);
    void addConfigSignal();
    void removeConfigSignal(int);

private:
    std::unique_ptr<QListWidget> _listWidget;

    QListWidgetItem* getItem();

private slots:
    void addButtonClicked();
    void removeButtonClicked();
    void changeConfigSlot(QListWidgetItem* current, QListWidgetItem* previous);
};


#endif //CONFIGSWIDGET_H
