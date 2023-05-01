#ifndef CONFIGSWIDGET_H
#define CONFIGSWIDGET_H

#include "UniversalString.h"
#include "DataTransferInterface.h"

#include <QWidget>
#include <QListWidget>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>


class ConfigsWidget : public QWidget
{
    Q_OBJECT

public:
    ConfigsWidget(QWidget* parent = nullptr);
};


#endif //CONFIGSWIDGET_H
